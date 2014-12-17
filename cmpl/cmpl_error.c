
void cmpl_error(void)
{
	struct msghdr hdr;
	hdr.sender = PROC_CMPL;
	hdr.receiver = PROC_COORD;
	hdr.info_level = PINFO_ERROR;
	hdr.msgtype = MT_INTERN;
	hdr.un.errno = errno;
	hdr.payload_len = 0;
	cmpl_send_coord(&hdr, sizeof(hdr));
}
