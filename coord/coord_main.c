#include "coord.h"
#include "proc.h"
#include "pinfo.h"

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

void coord_monit_child(void)
{
	int pid, wait_status;
	int i;

	for (i = PROC_CHILD_MIN; i <= PROC_CHILD_MAX; ++i) {
		pid = waitpid(coord_child_pid[i], &wait_status, WNOHANG);
		if (pid == -1) {
			pinfo(PINFO_WARN, TRUE,
			    "waitpid() for child pid %d failed",
			    coord_child_pid[i]);
		} else if (pid != 0) {
			if (WIFEXITED(wait_status))
				pinfo(PINFO_FATAL, FALSE,
				    "child %d exited with code %d\n",
				    coord_child_pid[i], WEXITSTATUS(wait_status));
			else if (WIFSIGNALED(wait_status))
				pinfo(PINFO_FATAL, FALSE,
				    "child %d received signal %d (%s)\n",
				    coord_child_pid[i], WTERMSIG(wait_status),
				    strsignal(WTERMSIG(wait_status)));
		}
	}
}

int coord_main(void)
{
	fd_set rd, wr, ex;
	FD_ZERO(&rd);
	FD_ZERO(&wr);
	FD_ZERO(&ex);

	int i;

	while (1) {
		/* stage 1: monitor if children processes are running */
		coord_monit_child();
		/* stage 2: read from children, should block */
		for (i = PROC_CHILD_MIN; i <= PROC_CHILD_MAX; ++i)
			FD_SET(coord_sockfd[i], &rd);

		if (select(PROC_NUM, &rd, &wr, &ex, NULL) >= 0) {
			for (i = PROC_CHILD_MIN; i <= PROC_CHILD_MAX; ++i)
				if (FD_ISSET(coord_sockfd[i], rd))
					coord_handle(i);
		} else {
			pinfo(PINFO_ERROR, TRUE, "select");
			coord_term(1);
		}
	}
}
