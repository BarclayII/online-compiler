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

static void *thread_entry(void *arg)
{
	struct _pool_task *task;
	pool_t *pool = (pool_t *)arg;
	for (;;) {
		limit_sem_wait(&(new->sem), pool_fetch_task, pool, (void **)&task);
		(task->fn)(task->data);
		free_n(&task);
	}
}

static void *pool_fetch_task(pool_t *pool)
{
	struct _pool_task *task;

	task = (struct _pool_task *)malloc(sizeof(struct _pool_task));

	if (task == NULL)
		return NULL;

	if (pthread_mutex_lock_n(&(pool->mutex)) != 0) {
		pinfo(PINFO_ERROR, TRUE, "acquiring thread pool lock failed");
		srvr_intern_error(PINFO_ERROR);
	}

	memcpy(task, pool->task[(pool->task_front)++], sizeof(struct _pool_task));

	if (pthread_mutex_unlock_n(&(pool->mutex)) != 0) {
		pinfo(PINFO_ERROR, TRUE, "releasing thread pool lock failed");
		srvr_intern_error(PINFO_ERROR);
	}

	return task;
}
