
void cmpl_main(void)
{
	struct msghdr hdr;
	for (;;) {
		if (read_n(fd, &hdr, sizeof(hdr)) == -1) {
			cmpl_error();
			pinfo(PINFO_ERROR, TRUE, "read");
		} else {
			/* the compiler process receives message from 
			 * server process, which contains the directory
			 * name */
			cmpl_process_msg(&hdr);
		}
	}
}
