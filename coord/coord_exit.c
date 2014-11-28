#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include "coord.h"

/*
 * coord_exit: exit coordinator.
 * Not recommended for direct use.
 */
int coord_exit(int retval)
{
	exit(retval);
}
