#include "srvr.h"
#include "pinfo.h"
#include "msg.h"

void srvr_intern_error(int level)
{
	struct msghdr hdr;
	hdr.sender = PROC_SRVR;
	hdr.receiver = PROC_COORD;
	hdr.info_level = level;
	hdr.msgtype = MT_INTERN;
	hdr.payload_len = 0;
	hdr.un.errno = errno;

	srvr_send_coord(&hdr, sizeof(hdr));
}
