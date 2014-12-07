#include "config.h"
#include "srvr.h"
#include "proc.h"
#include "pinfo.h"

void srvr_main(void)
{
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		pinfo(PINFO_ERROR, TRUE, "socket");
		srvr_intern_error(PINFO_ERROR);
	}
}
