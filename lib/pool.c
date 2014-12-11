#include "pool.h"

pool_t *pool_new(int max_task, int max_thread)
{
	pool_t *new = (pool_t)malloc(sizeof(pool_t));
	if (new == NULL)
		goto fail;

	new->max_task = max_task;
	new->max_thread = max_thread;

	new->task_front = new->task_rear = 0;

	new->task = 
	    (struct _pool_task *)calloc(max_task, sizeof(struct _pool_task));
	if (new->task == NULL)
		goto clear_pool;

	if (pthread_mutex_init(&(new->lock), NULL) != 0)
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
	int errno_saved = errno;
	for (i = 0; i < max_thread; ++i) {
		pthread_cancel(new->thread[i]);
	}
clear_sem:
	limit_sem_destroy(&(new->sem));
clear_thread:
	free_n(&(new->thread));
clear_mutex:
	pthread_mutex_destroy(&(new->lock));
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
	pthread_mutex_destroy(&(pool->lock));
	free_n(&(pool->task));
	free_n(&pool);
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
		limit_sem_wait(&(new->sem), &cbs);
		task = (struct _pool_task *)(cbs.ret);
		(task->fn)(task->data);
		free_n(&task);
	}

	return NULL;
}

static void *pool_fetch_task(pool_t *pool)
{
	struct _pool_task *task;

	task = (struct _pool_task *)malloc(sizeof(struct _pool_task));

	if (task == NULL)
		return NULL;

	if (pthread_mutex_lock_n(&(pool->mutex)) != 0) {
		free_n(&task);
		return NULL;
	}

	memmove(task, pool->task[(pool->task_front)++], sizeof(struct _pool_task));

	if (pthread_mutex_unlock_n(&(pool->mutex)) != 0) {
		free_n(&task);
		return NULL;
	}

	return task;
}

static struct _pool_with_task {
	pool_t *pool;
	struct _pool_task task;
};

static void *pool_submit_local(void *arg)
{
	struct _pool_with_task *pt = (struct _pool_with_task *)arg;
	pool_t *pool = pt->pool;

	if (pthread_mutex_lock_n(&(pool->mutex)) != 0) {
		free_n(&task);
		return NULL;
	}

	memmove(pool->task[(pool->task_rear)++], &(pt->task),
	    sizeof(struct _pool_task));

	if (pthread_mutex_unlock_n(&(pool->mutex)) != 0) {
		free_n(&task);
		return NULL;
	}

	return NULL;
}

int pool_submit(pool_t *pool, void (*)func(void *), void *arg)
{
	struct limit_sem_cb_struct cbs;
	struct _pool_with_task pt;
	pt.pool = pool;
	pt.task.fn = func;
	pt.task.data = arg;
	cbs.func = pool_submit_local;
	cbs.arg = &pt;
	cbs.ret = NULL;
	cbs.ret_size = 0;
	return limit_sem_trypost(&(pool->sem), &cbs);
}