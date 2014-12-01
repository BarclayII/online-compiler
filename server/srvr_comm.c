#include "srvr.h"
#include "pinfo.h"

void srvr_send_coord(void *buf, int len)
{
	if (write_n(coord_fd, buf, len) == -1) {
		pinfo(PINFO_FATAL, TRUE, "write() to COORD failed");
		srvr_term(1);
	}
}
