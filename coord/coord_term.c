#include <sys/types.h>
#include <signal.h>

int coord_exit(int retval)
{
	exit(retval);
}

/*
 * coord_kill: send signal to all children
 * Parameters:
 * @signum	signal number
 * @retval	-1 to continue execution, non-negative to terminate
 */
int coord_kill(int signum, int retval)
{
	int i;
	for (i = PROC_ID_MIN; i < PROC_ID_MAX; ++i) {
		if (pid_child[i] > 0)
			kill(pid_child[i], signum);
	}
	if (retval >= 0)
		coord_exit(retval);
}
