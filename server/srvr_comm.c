#include "srvr.h"
#include "pinfo.h"

void send_coord(void *buf, int len)
{
	if (write_n(coord_fd, buf, len) == -1) {
		pinfo(PINFO_FATAL, TRUE, "write() to COORD failed");
		srvr_term(1);
	}
}

void srvr_notify_cmpl(const char *dir)
{
	struct msghdr hdr;
	hdr.sender = PROC_SRVR;
	hdr.receiver = PROC_CMPL;
	hdr.info_level = PINFO_INFO;
	hdr.msgtype = MT_SUCCESS;
	hdr.payload_len = strlen(dir);
	srvr_send_coord(&hdr, sizeof(hdr));
	srvr_send_coord(dir, hdr.payload_len);
}
