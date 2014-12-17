
void cmpl_error(int level, int append, const char *fmt, ...)
{
	struct msghdr hdr;
	hdr.sender = PROC_CMPL;
	hdr.receiver = PROC_COORD;
	hdr.info_level = level;
	hdr.msgtype = MT_INTERN;
	hdr.un.errno = errno;
	hdr.payload_len = 0;
	cmpl_send_coord(&hdr, sizeof(hdr));

	va_list ap;
	va_start(ap, fmt);
	vpinfo(level, append, fmt, ap);
	va_end(ap);
}
