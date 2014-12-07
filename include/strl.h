#ifndef _STRL_H
#define _STRL_H

#ifndef LIBBSD
char *strlcpy(char *, const char *, size_t);
char *strlcat(char *, const char *, size_t);
#endif

char *strrtrim(char *);
char *strltrim(char *);
char *strtrim(char *);

#endif
