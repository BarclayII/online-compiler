#include "coord.h"
#include "pinfo.h"
#include "proc.h"

#include "cmpl.h"
#include "exec.h"
#include "srvr.h"

typedef void (*entry_t)(int);

static entry_t _entries[PROC_NUM_CHILD + 1] = {
	NULL,
	cmpl_enter,
	exec_enter,
	srvr_enter
};

int coord_to_child(int proc_type, int fd)
{
	_entries[proc_type](fd);
}
