#ifndef _LIMIT_SEM_H
#define _LIMIT_SEM_H

#include <semaphore.h>

typedef struct {
	sem_t sem_set, sem_unset;
} limit_sem_t;

int limit_sem_init(limit_sem_t *, int, unsigned int, unsigned int);
int limit_sem_destroy(limit_sem_t *);
int limit_sem_wait(limit_sem_t *, void (*)(void *), void *);
int limit_sem_trywait(limit_sem_t *, void (*)(void *), void *);
int limit_sem_timedwait(limit_sem_t *, const struct timespec *, void (*)(void *),
    void *);
int limit_sem_post(limit_sem_t *, void (*)(void *), void *);
int limit_sem_trypost(limit_sem_t *, void (*)(void *), void *);
int limit_sem_timedpost(limit_sem_t *, const struct timespec *, void (*)(void *),
    void *);
int limit_sem_getvalue(limit_sem_t *, int *);

#endif
