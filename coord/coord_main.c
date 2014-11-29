#include "coord.h"
#include "proc.h"
#include "pinfo.h"

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int coord_main(void)
{
	fd_set rd, wr, ex;
	FD_ZERO(&rd);
	FD_ZERO(&wr);
	FD_ZERO(&ex);

	int i;

	while (1) {
		for (i = PROC_CHILD_MIN; i <= PROC_CHILD_MAX; ++i)
			FD_SET(coord_sockfd[i], &rd);

		if (select(PROC_NUM, &rd, &wr, &ex, NULL) >= 0) {
			for (i = PROC_CHILD_MIN; i <= PROC_CHILD_MAX; ++i)
				if (FD_ISSET(coord_sockfd[i], rd))
					coord_handle(i);
		} else {
			pinfo(PINFO_ERROR, TRUE, "select");
			coord_term(1);
		}
	}
}
