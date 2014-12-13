#include "config.h"
#include "srvr.h"
#include "proc.h"
#include "pinfo.h"

void srvr_spinup(void *arg)
{
	int client_fd = *(int *)arg;
}

void srvr_main(void)
{
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		pinfo(PINFO_ERROR, TRUE, "socket");
		srvr_intern_error(PINFO_ERROR);
	}

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = SRVR_PORT;
	sin.sin_addr.s_addr = INADDR_ANY;
	if (bind(listen_fd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
		pinfo(PINFO_ERROR, TRUE, "bind");
		srvr_intern_error(PINFO_ERROR);
	}

	if (listen(listen_fd, SRVR_MAXCONN) == -1) {
		pinfo(PINFO_ERROR, TRUE, "listen");
		srvr_intern_error(PINFO_ERROR);
	}

	int sock;
	while ((sock = accept(listen_fd, (struct sockaddr *)&sin, sizeof(sin)))
	    != -1) {
		pool_submit(srvr_pool, srvr_spinup, &sock);
	}
}
