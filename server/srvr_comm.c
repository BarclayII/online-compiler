#include "srvr.h"
#include "pinfo.h"

void srvr_notify_cmpl(const char *dir)
{
	struct msghdr hdr;
	hdr.sender = PROC_SRVR;
	hdr.receiver = PROC_CMPL;
	hdr.info_level = PINFO_INFO;
	hdr.msgtype = MT_SUCCESS;
	hdr.payload_len = strlen(dir);
	send_coord(&hdr, sizeof(hdr));
	send_coord(dir, hdr.payload_len);
}
