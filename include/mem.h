#ifndef _MEM_H
#define _MEM_H

#include <stdlib.h>

#define free_n(ptr)	\
	do { \
		free(ptr); \
		ptr = NULL; \
	} while (0)

#endif
