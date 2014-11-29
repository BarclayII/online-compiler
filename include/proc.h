#ifndef _PROC_H
#define _PROC_H

#define PROC_COORD	0
#define PROC_CMPL	1
#define PROC_EXEC	2
#define PROC_SRVR	3

#define PROC_CHILD_MIN	PROC_CMPL
#define PROC_CHILD_MAX	PROC_SRVR
#define PROC_NUM_CHILD	(PROC_CHILD_MAX - PROC_CHILD_MIN + 1)
#define PROC_NUM	(PROC_CHILD_MAX + 1)

extern int current_proc;		/* current process */

#endif
