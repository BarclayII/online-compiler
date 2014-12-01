#include <errno.h>
#include <string.h>
#include "proc.h"
#include "pinfo.h"
#include "bool.h"

static char *strlevel[] = {
	NULL,
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL",
	"PANIC"
};

static char *strproc[] = {
	"COORD",
	"CMPLR",
	"EXECT",
	"SERVR"
}

void pinfo(int level, bool append, const char *fmt, ...)
{
	va_list vp;

	va_start(vp, fmt);
	vpinfo(level, append, fmt, vp);
	va_end(vp);
}

void vpinfo(int level, bool append, const char *fmt, va_list vp)
{
	printf("[%s] %s: ", strproc[current_proc], strlevel[level]);
	vprintf(fmt, vp);
	if (append)
		printf(": %s", strerror(errno));
	printf("\n");
}
