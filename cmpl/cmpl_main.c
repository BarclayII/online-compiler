
void cmpl_main(void)
{
	struct msghdr hdr;
	for (;;) {
		if (read_n(fd, &hdr, sizeof(hdr)) == -1) {
			cmpl_error();
			pinfo(PINFO_ERROR, TRUE, "read");
		}
	}
}
