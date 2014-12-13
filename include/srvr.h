#ifndef _SRVR_H
#define _SRVR_H

#include "pool.h"

extern pool_t *srvr_pool;

extern int coord_fd;			/* FD for socket to coordinator */
extern int listen_fd;			/* FD for listener */

void srvr_enter(int);

void srvr_init(void);
void srvr_intern_error(int);

#endif
