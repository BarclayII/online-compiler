#ifndef _PATH_H
#define _PATH_H

#include <stdio.h>
#include <stdarg.h>

char *build_path(const char *, const char *);
char *build_multi_level_path(const char *, int, ...);

FILE *fcreat(const char *, const char *);

#endif
