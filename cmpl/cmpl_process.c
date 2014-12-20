
static const char *ldflags = NULL;
static const char *cflags = NULL;

void cmpl_process_msg(struct msghdr *hdr)
{
	switch (hdr->sender) {
	case PROC_COORD:
		/* FIXME */
		break;
	case PROC_SRVR:
		char *dir = (char *)malloc(hdr->payload_len);
		if (read_n(coord_fd, dir, hdr->payload_len) == -1) {
			cmpl_error(MT_INTERN, PINFO_ERROR, TRUE, "read");
		}
		cmpl_do(dir);
		break;
	}
}

void cmpl_process(char *filename)
{
	switch (getextension(filename)) {
	case FILE_CSOURCE:
		cmpl_process_csource(filename);
		break;
	case FILE_CHEADER:
		break;
	}
}
