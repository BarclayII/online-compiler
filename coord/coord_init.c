#include <errno.h>
#include "coord.h"
#include "proc.h"

#define NUM_SOCKFD	(PROC_NUM_CHILD * 2 + 2)

static int _sockfd[NUM_SOCKFD + 2];

static int coord_create_sockets(void)
{
	int i;

	for (i = PROC_CHILD_MIN; i <= PROC_CHILD_MAX; ++i) {
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, _sockfd + i*2) == -1) {
			pinfo(PINFO_ERROR, TRUE, "socketpair");
			return -1;
		}
	}

	return 0;
}

static int coord_create_processes(void)
{
	int i;
	pid_t pid;

	for (i = PROC_CHILD_MIN; i <= PROC_CHILD_MAX; ++i) {
		if ((pid = fork()) == -1) {
			pinfo(PINFO_ERROR, TRUE, "fork");
			return -1;
		} else if (pid == 0) {
			/* setup current process type */
			current_proc = i;
			/* close all sockets except the one for communication with
			 * the coordinator */
			int j;
			for (j = PROC_CHILD_MIN; j <= PROC_CHILD_MAX; ++j) {
				if (close(_sockfd[j * 2]) == -1)
					pinfo(PINFO_WARN, TRUE,
					    "failed to close unused sockets");
				if ((j != i) && close(_sockfd[j * 2 + 1]) == -1)
					pinfo(PINFO_WARN, TRUE,
					    "failed to close unused sockets");
			}
			/* pass the file descriptor to children */
			coord_to_child(i, _sockfd[i * 2 + 1]);
			/* NOTREACHED */
			break;
		} else {
			coord_pid_child[i] = pid;
			coord_sockfd[i] = _sockfd[i * 2];
			if (close(_sockfd[i * 2 + 1]) == -1)
				pinfo(PINFO_WARN, TRUE,
				    "failed to close unused sockets");
		}
	}

	return 0;
}

int coord_init(void)
{
	memset(coord_pid_child, 0, sizeof(coord_pid_child));
	memset(coord_sockfd, 0, sizeof(coord_sockfd));

	if (coord_create_sockets() != 0)
		coord_exit(1);

	if (coord_create_processes() != 0)
		/* terminate possibly-created children */
		coord_term(1);

	current_proc = PROC_COORD;

	return 0;
}
