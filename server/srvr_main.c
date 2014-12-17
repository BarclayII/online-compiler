#include "config.h"
#include "srvr.h"
#include "proc.h"
#include "pinfo.h"

void srvr_spinup(void *arg)
{
	int client_fd = *(int *)arg;
	FILE *fp = fdopen(client_fd, "r+");
	if (fp == NULL) {
		srvr_error(MT_INTERN, PINFO_ERROR, TRUE, "fdopen");
		goto finish;
	}

	char *dir;

	yyscan_t scanner;
	yylex_init(&scanner); {
		yyset_in(fp, scanner);
		dir = yylex(client_fd, scanner);
	} yylex_destroy(scanner);

	srvr_notify_cmpl(dir);

finish:
	if (fp != NULL)
		fclose(fp);
}

void srvr_main(void)
{
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		srvr_error(MT_INTERN, PINFO_ERROR, TRUE, "socket");
	}

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = SRVR_PORT;
	sin.sin_addr.s_addr = INADDR_ANY;
	if (bind(listen_fd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
		srvr_error(MT_INTERN, PINFO_ERROR, TRUE, "bind");
	}

	if (listen(listen_fd, SRVR_MAXCONN) == -1) {
		srvr_error(MT_INTERN, PINFO_ERROR, TRUE, "listen")
	}

	int sock;
	for (;;) {
		sock = accept(listen_fd, (struct sockaddr *)&sin, &addr_size);
		if (sock == -1) {
			case EAGAIN:
			case EINTR:
				break;
			case ECONNABORTED:
				pinfo(PINFO_WARN, TRUE, "accept");
				break;
			default:
				pinfo(PINFO_ERROR, TRUE, "accept");
				break;
		} else {
			if (pool_submit(pool, srvr_spinup, &sock, sizeof(int), 0)
			    != 0) {
				close(sock);
				pinfo(PINFO_WARN, TRUE, "submit");
			}
		}
	}
}
