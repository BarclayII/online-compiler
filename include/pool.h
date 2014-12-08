#ifndef _POOL_H
#define _POOL_H

/*
 * A thread pool implementation
 */

#include "limit_sem.h"

struct _pool_task {
	void (*fn)(void *);
	void *data;
};

struct _pool {
	int max_task, max_thread;
	struct _pool_task *task;
	pthread_mutex_t lock;
	pthread_t *thread;
	limit_sem_t sem;
};

typedef struct _pool pool_t;

/*
 * Initialize a new thread pool and spin up threads.
 */
pool_t *pool_new(int max_task, int max_thread);

/*
 * Terminate the thread pool.
 */
int pool_finalize(pool_t *pool);

/*
 * Submit a task to thread pool.
 */
int pool_submit(pool_t *pool, void (*)func(void *), void *arg);

#endif
