#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "strl.h"
#include "path.h"

/*
 * build_path concatenates @dir and @file into a pathspec.
 */
char *
build_path(const char *file, const char *dir)
{
	int path_len = strlen(dir) + strlen(file) + 1;
	char *path = (char *)calloc(path_len + 1);
	if (path == NULL) {
		pinfo(PINFO_WARN, TRUE, "calloc");
		return -1;
	}

	strlcpy(path, dir, path_len + 1);
	strlcat(path, PATH_SEP, path_len + 1);
	strlcat(path, file, path_len + 1);

	return path;
}

char *
build_multi_level_path(const char *file, int num_level, ...)
{
	va_list ap;
	char *p = strdup(file), *q;
	int len = strlen(p), lq, ls, i;
	va_start(ap, num_level);
	ls = strlen(PATH_SEP);
	for (i = 0; i < num_level; ++i) {
		q = va_arg(ap, char *);
		lq = strlen(q);
		len += lq + ls;
		p = realloc(p, len + 1);
		memmove(p + lq + ls, p, len - lq - ls + 1);
		memcpy(p, q, lq);
		memcpy(p + lq, PATH_SEP, ls);
	}
	va_end(ap);
	return p;
}

#define O_PERM_CREAT	(O_RDWR | O_CREAT | O_TRUNC | O_EXCL)

FILE *
fcreat(const char *path, const char *mode)
{
	int fd;
	FILE *fp;
	
	if ((fd = open(path, O_PERM_CREAT, DFL_UMASK)) == -1) {
		pinfo(PINFO_WARN, TRUE, "open %s", path);
		return NULL;
	}

	if ((fp = fdopen(fd, mode)) == NULL)
		pinfo(PINFO_WARN, TRUE, "fdopen %s", path);

	return fp;
}
