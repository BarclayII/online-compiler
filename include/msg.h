#ifndef _MSG_H
#define _MSG_H

#include "proc.h"

struct msghdr {
	int sender;
	int receiver;
	int info_level;
	int msgtype;
	union {
		int errno;	/* MT_INTERN */
		int retcode;	/* MT_CHILD_EXITED or alike */
		int signal;	/* MT_CHILD_SIGNALLED or alike */
	} un;
	int payload_len;
};

#define MT_SUCCESS	0	/* Nothing special, reserved */
#define MT_INTERN	1	/* Component internal error */
#define MT_CMPL_FAIL	2	/* Failed to compile, compiler message in 
				 * payload */
#define MT_CHILD_EXITED	3	/* Executor's child exited with return code stored
				 * in un.retcode */
#define MT_CHILD_SIGNALED 4	/* Executor's child signaled */

void _error(int, int, int, const char *, ...);
#define srvr_error _error
#define cmpl_error _error
void send_coord(void *, int);

#endif
