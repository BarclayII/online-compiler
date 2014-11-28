#include <errno.h>
#include <err.h>
#include "coord.h"
#include "proc.h"

int coord_init(void)
{
	if ((errno = coord_create_sockets()) != 0)
		coord_exit(1);

	if ((errno = coord_create_processes()) != 0)
		/* terminate possibly-created children */
		coord_term(1);

	if ((errno = coord_init_socket()) != 0)
		coord_warn("failed to close unused sockets");
}
