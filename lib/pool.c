#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include "pool.h"
#include "mem.h"
#include "limit_sem.h"
#include "multithread.h"

static void *thread_entry(void *arg);

pool_t *pool_new(int max_task, int max_thread)
{
	int errno_saved;
	pool_t *new = (pool_t *)malloc(sizeof(pool_t));
	if (new == NULL)
		goto fail;

	new->max_task = max_task;
	new->max_thread = max_thread;

	new->task_front = new->task_rear = 0;

	new->task = 
	    (struct _pool_task **)calloc(max_task, sizeof(struct _pool_task *));
	if (new->task == NULL)
		goto clear_pool;

	if (pthread_mutex_init(&(new->mutex), NULL) != 0)
		goto clear_task;

	new->thread = (pthread_t *)calloc(max_thread, sizeof(pthread_t));
	if (new->thread == NULL)
		goto clear_mutex;

	if (limit_sem_init(&(new->sem), 0, max_task, 0) != 0)
		goto clear_thread;

	int i;
	for (i = 0; i < max_thread; ++i)
		if (pthread_create(&(new->thread[i]), NULL, 
		    thread_entry, new) != 0) {
			goto term_threads;
		}

	return new;

term_threads:
	errno_saved = errno;
	for (i = 0; i < max_thread; ++i) {
		pthread_cancel(new->thread[i]);
	}
clear_sem:
	limit_sem_destroy(&(new->sem));
clear_thread:
	free_n(&(new->thread));
clear_mutex:
	pthread_mutex_destroy(&(new->mutex));
clear_task:
	free_n(&(new->task));
clear_pool:
	free_n(&new);
fail:
	return NULL;
}

void pool_terminate(pool_t *pool)
{
	int i;
	for (i = 0; i < pool->max_thread; ++i)
		pthread_cancel(pool->thread[i]);
	limit_sem_destroy(&(pool->sem));
	free_n(&(pool->thread));
	pthread_mutex_destroy(&(pool->mutex));
	free_n(&(pool->task));
	free_n(&pool);
}

static void *pool_fetch_task(void *arg)
{
	pool_t *pool = (pool_t *)arg;

	if (pthread_mutex_lock_n(&(pool->mutex)) != 0) {
		return NULL;
	}

	struct _pool_task *current_task = pool->task[pool->task_front];
	++(pool->task_front);
	if (pool->task_front >= pool->max_task)
		pool->task_front -= pool->max_task;

	if (pthread_mutex_unlock_n(&(pool->mutex)) != 0) {
		return NULL;
	}

	return current_task;
}

static void *thread_entry(void *arg)
{
	struct _pool_task *task;
	pool_t *pool = (pool_t *)arg;

	struct limit_sem_cb_struct cbs;

	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	cbs.func = pool_fetch_task;
	cbs.arg = pool;
	cbs.ret_size = 0;

	for (;;) {
		limit_sem_wait(&(pool->sem), &cbs);
		task = (struct _pool_task *)(cbs.ret);
		(task->fn)(task->data);
		free_n(&task);
	}

	return NULL;
}

struct _pool_with_task {
	pool_t *pool;
	struct _pool_task *task;
};

static void *pool_submit_local(void *arg)
{
	struct _pool_with_task *pt = (struct _pool_with_task *)arg;
	pool_t *pool = pt->pool;

	if (pthread_mutex_lock_n(&(pool->mutex)) != 0) {
		return NULL;
	}

	pool->task[(pool->task_rear)++] = pt->task;
	if (pool->task_rear >= pool->max_task)
		pool->task_rear -= pool->max_task;

	if (pthread_mutex_unlock_n(&(pool->mutex)) != 0) {
		return NULL;
	}

	return NULL;
}

int pool_submit(pool_t *pool, void (*func)(void *), void *arg, size_t len, int copy)
{
	struct limit_sem_cb_struct cbs;
	struct _pool_with_task pt;
	pt.pool = pool;
	pt.task = (struct _pool_task *)malloc(sizeof(struct _pool_task));
	pt.task->fn = func;
	if (!copy)
		pt.task->data = arg;
	else {
		pt.task->data = malloc(len);
		memmove(pt.task->data, arg, len);
	}
	pt.task->len = len;
	cbs.func = pool_submit_local;
	cbs.arg = &pt;
	cbs.ret = NULL;
	cbs.ret_size = 0;
	return limit_sem_trypost(&(pool->sem), &cbs);
}
