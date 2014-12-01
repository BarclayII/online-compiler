#include <unistd.h>
#include <errno.h>

/*
 * A more robust unbuffered read/write routine
 */
int read_n(int fd, void *buf, size_t len)
{
	void *curp = buf;
	size_t remain;
	ssize_t result;

	for (remain = len; remain > 0; ) {
		result = read(fd, curp, remain);
		if (result == -1) {
			switch (errno) {
			case EINTR:
			case EAGAIN:
			case EWOULDBLOCK:
				break;
			default:
				/* errno set by read() */
				return -1;
			}
		} else if (result == 0) {
			/* EOF */
			break;
		} else {
			curp += result;
			remain -= result;
		}
	}

	return curp - buf;
}

int write_n(int fd, const void *buf, size_t len)
{
	void *curp = buf;
	size_t remain;
	ssize_t result;

	for (remain = len; remain > 0; ) {
		result = write(fd, curp, remain);
		if (result == -1) {
			switch (errno) {
			case EINTR:
			case EAGAIN:
			case EWOULDBLOCK:
				break;
			default:
				/* errno set by write() */
				return -1;
			}
		} else {
			curp += result;
			remain -= result;
		}
	}

	return curp - buf;
}
