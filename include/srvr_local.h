#ifndef _SRVR_LOCAL_H
#define _SRVR_LOCAL_H

#include <stdio.h>

char *srvr_mkdir(const char *);
int srvr_set_outexec(const char *, const char *);
int srvr_set_ldflags(const char *, const char *);
FILE *srvr_open_file(const char *, const char *);
int srvr_set_cflags(const char *, const char *, const char *);
int srvr_set_lexout(const char *, const char *, const char *);
int srvr_set_yaccout(const char *, const char *, const char *);
int srvr_puts(FILE *, const char *);

#endif
