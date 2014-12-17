
void send_error(int type, int level)
{
	struct msghdr hdr;
	hdr.sender = current_proc;
	hdr.receiver = PROC_COORD;
	hdr.info_level = level;
	hdr.msgtype = type;
	hdr.un.errno = errno;
	hdr.payload_len = 0;
	send_coord(&hdr, sizeof(hdr));
}

void _error(int type, int level, int append, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	send_error(type, level);
	vpinfo(level, append, fmt, ap);
	va_end(ap);
}

void send_coord(void *buf, int len)
{
	if (current_proc == PROC_COORD) {
		pinfo(PINFO_WARN, FALSE, "coordinator sending to itself?");
		return;
	}
	if (write_n(coord_fd, buf, len) == -1) {
		pinfo(PINFO_FATAL, TRUE, "write() to COORD failed");
		srvr_term(1);
	}
}
