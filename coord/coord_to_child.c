#include "coord.h"
#include "pinfo.h"
#include "proc.h"

#include "cmpl.h"
#include "exec.h"
#include "srvr.h"

typedef void (*entry_t)(void);

static entry_t _entries[PROC_ID_MAX + 1] = {
	NULL,
	cmpl_enter,
	exec_enter,
	srvr_enter
};

int coord_to_child(int proc_type)
{
	_entries[proc_type]();
}
