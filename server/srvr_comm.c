#include "srvr.h"
#include "pinfo.h"

void srvr_send_coord(void *buf, int len)
{
	while (write(coord_fd, buf, len) == -1)
		switch (errno) {
		case EINTR:
			/* may need to do some additional work */
		case EAGAIN:
		case EWOULDBLOCK:
			break;
		default:
			pinfo(PINFO_FATAL, TRUE, "write() to COORD failed");
			srvr_term(1);
		}
}
