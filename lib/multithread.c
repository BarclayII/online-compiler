#include <pthread.h>
#include "multithread.h"

int pthread_mutex_lock_n(pthread_mutex_t *mutex)
{
	while (pthread_mutex_lock(mutex) != 0) {
		switch (errno) {
		case EAGAIN:
			break;
		default:
			return -errno;
		}
	}
	return 0;
}

int pthread_mutex_trylock_n(pthread_mutex_t *mutex)
{
	while (pthread_mutex_trylock(mutex) != 0) {
		switch (errno) {
		case EAGAIN:
			break;
		default:
			return -errno;
		}
	}
	return 0;
}

