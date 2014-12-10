#ifndef _MULTITHREAD_H
#define _MULTITHREAD_H

#include <pthread.h>

int pthread_mutex_lock_n(pthread_mutex_t *mutex);
int pthread_mutex_trylock_n(pthread_mutex_t *mutex);
#define pthread_mutex_unlock_n(mutex)	pthread_mutex_unlock(mutex)

#endif
