/*
 * Header for coordinator sources
 */
#ifndef _COORD_H
#define _COORD_H

#include <signal.h>
#include <stdarg.h>

extern int sockfd[4];		/* domain sockets, 0 is not used */

extern pid_t pid_child[4];	/* children pids */

int coord_exit(int);		/* abruptly kill coordinator - dangerous, internal
				 * only.
				 */
int coord_kill(int, int);	/* abruptly kill coordinator and children */
#define coord_instakill(r)	coord_kill(SIGKILL, r);
int coord_init(void);		/* initialize */
int coord_warn(const char *, ...);

#endif
