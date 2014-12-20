#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "const.h"
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
		return NULL;
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
		if (p == NULL) {
			pinfo(PINFO_WARN, TRUE, "realloc");
			return NULL;
		}
		memmove(p + lq + ls, p, len - lq - ls + 1);
		memcpy(p, q, lq);
		memcpy(p + lq, PATH_SEP, ls);
	}
	va_end(ap);
	return p;
}

/*
 * creat() equivalent for FILE streams
 * creates directory if needed.
 */
FILE *
fcreat(const char *path, const char *mode)
{
	int fd;
	FILE *fp;

	int cdir_fd;
	if ((cdir_fd = open(".", O_RDWR | O_DIRECTORY)) == -1) {
		pinfo(PINFO_DEBUG, TRUE, "open current directory failed");
		return NULL;
	}

	char *p = strdup(path), *p_end = p + strlen(p);
	char *d;

	for (d = strtok(p, PATH_SEP);
	    (d != NULL) && (d + strlen(d) != p_end);
	    d = strtok(NULL, PATH_SEP)) {
		if (chdir(d) == -1)
			if (errno == ENOENT) {
				if (mkdir(d, DFL_UMASK_DIR) == -1) {
					pinfo(PINFO_DEBUG, TRUE, "mkdir %s", d);
					return NULL;
				}
				if (chdir(d) == -1) {
					pinfo(PINFO_DEBUG, TRUE,
					    "chdir %s after mkdir", d);
					return NULL;
				}
			} else {
				pinfo(PINFO_DEBUG, TRUE, "chdir %s", d);
				return NULL;
			}
	}

	if (fchdir(cdir_fd) == -1) {
		pinfo(PINFO_DEBUG, TRUE, "change to current directory failed");
		return NULL;
	}

	if ((fd = open(path, O_PERM_CREAT, DFL_UMASK)) == -1) {
		if (errno != ENOENT) {
			pinfo(PINFO_DEBUG, TRUE, "open %s", path);
			return NULL;
		}
	}

	if ((fp = fdopen(fd, mode)) == NULL)
		pinfo(PINFO_DEBUG, TRUE, "fdopen %s", path);

	if (close(cdir_fd) == -1)
		pinfo(PINFO_DEBUG, TRUE, "close current directory failed");

	return fp;
}

#undef O_PERM_CREAT
