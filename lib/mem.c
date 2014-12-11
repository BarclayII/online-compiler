#include <stdlib.h>
#include "mem.h"

/*
 * A free() wrapper which sets the pointer to NULL after freeing
 */
void free_n(void **buf_addr)
{
	free(*buf_addr);
	*buf_addr = NULL;
}
