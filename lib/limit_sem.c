/* $limit_sem.c$
 * This file is to provide semaphore function equivalents for "limited"
 * variants.
 *
 * A limited semaphore is a semaphore with given maximum semaphore value.
 *
 * This implementation is not robust.
 */
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include "limit_sem.h"
#include "mem.h"

int
limit_sem_init(limit_sem_t *sem, int pshared, unsigned int maximum, 
    unsigned int initial)
{
	if (sem_init(&(sem->sem_set), pshared, initial) != 0)
		return -1;
	if (sem_init(&(sem->sem_unset), pshared, maximum - initial) != 0)
		return -1;
	return 0;
}

int
limit_sem_destroy(limit_sem_t *sem)
{
	if (sem_destroy(&(sem->sem_set)) != 0)
		return -1;
	if (sem_destroy(&(sem->sem_unset)) != 0)
		return -1;
	return 0;
}

static void
limit_sem_process_cbs(struct limit_sem_cb_struct *cbs)
{
	if (cbs->func != NULL)
		if (cbs->ret_size == 0 || cbs->ret == NULL)
			cbs->ret = (*(cbs->func))(cbs->arg);
		else {
			void *tmp = (*(cbs->func))(cbs->arg);
			memmove(cbs->ret, tmp, cbs->ret_size);
			free_n(&tmp);
		}
}

int
limit_sem_wait(limit_sem_t *sem, struct limit_sem_cb_struct *cbs)
{
	if (sem_wait(&(sem->sem_set)) != 0)
		return -1;
	limit_sem_process_cbs(cbs);
	if (sem_post(&(sem->sem_unset)) != 0)
		return -1;
	return 0;
}

int
limit_sem_trywait(limit_sem_t *sem, struct limit_sem_cb_struct *cbs)
{
	if (sem_trywait(&(sem->sem_set)) != 0)
		return -1;
	limit_sem_process_cbs(cbs);
	if (sem_post(&(sem->sem_unset)) != 0)
		return -1;
	return 0;
}

int
limit_sem_timedwait(limit_sem_t *sem, const struct timespec *abs_timeout,
    struct limit_sem_cb_struct *cbs)
{
	if (sem_timedwait(&(sem->sem_set), abs_timeout) != 0)
		return -1;
	limit_sem_process_cbs(cbs);
	if (sem_post(&(sem->sem_unset)) != 0)
		return -1;
	return 0;
}

int
limit_sem_post(limit_sem_t *sem, struct limit_sem_cb_struct *cbs)
{
	if (sem_wait(&(sem->sem_unset)) != 0)
		return -1;
	limit_sem_process_cbs(cbs);
	if (sem_post(&(sem->sem_set)) != 0)
		return -1;
	return 0;
}

int
limit_sem_trypost(limit_sem_t *sem, struct limit_sem_cb_struct *cbs)
{
	if (sem_trywait(&(sem->sem_unset)) != 0)
		return -1;
	limit_sem_process_cbs(cbs);
	if (sem_post(&(sem->sem_set)) != 0)
		return -1;
	return 0;
}

int
limit_sem_timedpost(limit_sem_t *sem, const struct timespec *abs_timeout,
    struct limit_sem_cb_struct *cbs)
{
	if (sem_timedwait(&(sem->sem_unset), abs_timeout) != 0)
		return -1;
	limit_sem_process_cbs(cbs);
	if (sem_post(&(sem->sem_set)) != 0)
		return -1;
}

int
limit_sem_getvalue(limit_sem_t *sem, int *sval)
{
	if (sem_getvalue(&(sem->sem_set), sval) != 0)
		return -1;
	return 0;
}
