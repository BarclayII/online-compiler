#include "srvr.h"
#include "proc.h"
#include "pinfo.h"
#include "config.h"

void srvr_init_priv(void)
{
	if (chdir(ROOTDIR) == -1) {
		srvr_intern_error(PINFO_ERROR);
		pinfo(PINFO_ERROR, TRUE, "chdir(\"%s\") failed", SRVR_ROOT);
	}
	if (chroot(".") == -1) {
		srvr_intern_error(PINFO_ERROR);
		pinfo(PINFO_ERROR, TRUE, "chroot to current directory failed");
	}
	if (setuid(UID_SRVR) == -1) {
		srvr_intern_error(PINFO_ERROR);
		pinfo(PINFO_ERROR, TRUE, "setuid");
	}
}

void srvr_init_pool(void)
{
	pool = pool_init(SRVR_MAXTASK, SRVR_MAXTHREAD);
}

void srvr_enter(int fd)
{
	coord_fd = fd;

	/* initialize privilege e.g. setting up working directory and user id */
	srvr_init_priv();

	/* initialize pool */
	srvr_init_pool();

	/* enter main routine */
	srvr_main();
}
