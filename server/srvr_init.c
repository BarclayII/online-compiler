#include "srvr.h"
#include "proc.h"
#include "pinfo.h"
#include "config.h"

void srvr_init_priv(void)
{
	if (chdir(ROOTDIR) == -1) {
		pinfo(PINFO_ERROR, TRUE, "chdir(\"%s\") failed", SRVR_ROOT);
		srvr_intern_error(PINFO_ERROR);
	}
	if (chroot(".") == -1) {
		pinfo(PINFO_ERROR, TRUE, "chroot to current directory failed");
		srvr_intern_error(PINFO_ERROR);
	}
	if (setuid(UID_SRVR) == -1) {
		pinfo(PINFO_ERROR, TRUE, "setuid");
		srvr_intern_error(PINFO_ERROR);
	}
}

void srvr_enter(int fd)
{
	coord_fd = fd;

	/* initialize privilege e.g. setting up working directory and user id */
	srvr_init_priv();

	/* enter main routine */
	srvr_main();
}
