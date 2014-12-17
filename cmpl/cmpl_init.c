#include "cmpl.h"
#include "pinfo.h"

void cmpl_init(void)
{
	if (setuid(UID_CMPL) == -1) {
		cmpl_error();
		pinfo(PINFO_ERROR, TRUE, "setuid");
	}
}

void cmpl_enter(int fd)
{
	coord_fd = fd;

	cmpl_init();

	cmpl_main();
}