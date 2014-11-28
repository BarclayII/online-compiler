#include <errno.h>
#include "coord.h"
#include "proc.h"

#define NUM_SOCKFD	((PROC_ID_MAX - PROC_ID_MIN + 1) * 2)

static int _sockfd[NUM_SOCKFD + 2];

static int coord_create_sockets(void)
{
	int i;

	for (i = PROC_ID_MIN; i <= PROC_ID_MAX; ++i) {
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, _sockfd + i*2) == -1) {
			coord_pinfo(PINFO_ERROR, TRUE, "socketpair");
			return -1;
		}
	}

	return 0;
}

static int coord_create_processes(void)
{
	int i;
	pid_t pid;

	for (i = PROC_ID_MIN; i <= PROC_ID_MAX; ++i) {
		if ((pid = fork()) == -1) {
			coord_pinfo(PINFO_ERROR, TRUE, "fork");
			return -1;
		} else if (pid == 0) {
			coord_to_child(i);
			/* NOTREACHED */
			break;
		} else {
			coord_pid_child[i] = pid;
			coord_sockfd[i] = _sockfd[i * 2];
			if (close(_sockfd[i * 2 + 1]) == -1)
				coord_pinfo(PINFO_WARN, TRUE,
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

	return 0;
}
