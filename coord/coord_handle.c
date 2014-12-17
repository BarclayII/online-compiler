#include "coord.h"
#include "proc.h"
#include "pinfo.h"
#include "rw.h"
#include <unistd.h>

static void coord_handle_read(int fd, void *buf, size_t len)
{
	if (read_n(fd, buf, len) == -1) {
		pinfo(PINFO_ERROR, TRUE, "read");
		coord_term(1);
	}
}

static void coord_handle_write(int fd, const void *buf, size_t len)
{
	if (write_n(fd, buf, len) == -1) {
		pinfo(PINFO_ERROR, TRUE, "write");
		coord_term(1);
	}
}

int coord_handle(int proc_id)
{
	int fd = coord_sockfd[proc_id];
	struct msghdr hdr;
	
	/* read interprocess message header */
	coord_handle_read(fd, &hdr, sizeof(hdr));

	/* read payload */
	if (hdr.payload_len != 0) {
		char *payload = (char *)malloc(hdr.payload_len);
		if (payload == NULL) {
			pinfo(PINFO_ERROR, TRUE, "malloc");
			coord_term(1);
		}
		coord_handle_read(fd, payload, hdr.payload_len);
	}

	if (hdr.receiver != PROC_COORD) {
		/* forward messages whose destination is not the coordinator */
		int target_fd = coord_sockfd[hdr.receiver];
		coord_handle_write(target_fd, &hdr, sizeof(hdr));
		coord_handle_write(target_fd, payload, hdr.payload_len);
	} else {
		/* TODO: add child message handler here... */
	}
}
