#ifndef _PATH_H
#define _PATH_H

#include <stdarg.h>

#define PATH_SEP	"/"

char *build_path(const char *, const char *);
char *build_multi_level_path(const char *, int, ...);

#endif
