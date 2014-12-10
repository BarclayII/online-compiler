#ifndef _LIMIT_SEM_H
#define _LIMIT_SEM_H

#include <semaphore.h>

typedef struct {
	sem_t sem_set, sem_unset;
} limit_sem_t;

struct limit_sem_cb_struct {
	void *(*func)(void *);		/* callback function */
	void *arg;			/* callback function argument */
	void *ret;			/* callback function return address */
	size_t ret_size;		/* size of the return value, reserved */
};

int limit_sem_init(limit_sem_t *, int, unsigned int, unsigned int);
int limit_sem_destroy(limit_sem_t *);
int limit_sem_wait(limit_sem_t *, struct limit_sem_cb_struct *);
int limit_sem_trywait(limit_sem_t *, struct limit_sem_cb_struct *);
int limit_sem_timedwait(limit_sem_t *, const struct timespec *,
    struct limit_sem_cb_struct *);
int limit_sem_post(limit_sem_t *, struct limit_sem_cb_struct *);
int limit_sem_trypost(limit_sem_t *, struct limit_sem_cb_struct *);
int limit_sem_timedpost(limit_sem_t *, const struct timespec *,
    struct limit_sem_cb_struct *);
int limit_sem_getvalue(limit_sem_t *, int *);

#endif
