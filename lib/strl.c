#include <string.h>
#include <ctype.h>
#include "strl.h"

#ifndef LIBBSD

/*
 * strlcpy() and strlcat() are borrowed from OpenBSD source code.
 */

/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t
strlcpy(char *dst, const char *src, size_t siz)
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0) {
		while (--n != 0) {
			if ((*d++ = *s++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++)
			;
	}

	return(s - src - 1);	/* count does not include NUL */
}

/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz <= strlen(dst)).
 * Returns strlen(src) + MIN(siz, strlen(initial dst)).
 * If retval >= siz, truncation occurred.
 */
size_t
strlcat(char *dst, const char *src, size_t siz)
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end */
	while (n-- != 0 && *d != '\0')
		d++;
	dlen = d - dst;
	n = siz - dlen;

	if (n == 0)
		return(dlen + strlen(s));
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return(dlen + (s - src));	/* count does not include NUL */
}

#endif

char *
strrtrim(char *str)
{
	char *s = str;
	size_t n = strlen(s);
	size_t i;
	
	for (i = n - 1; (i >= 0) && isspace(s[i]); s[i--] = '\0')
		/* nothing */;
	
	return s;
}

#define __WHITESPACE " \t\n\v\f\r"
char *
strltrim(char *str)
{
	char *s = str;
	size_t lw = strspn(s, __WHITESPACE);
	size_t len = strlen(s);
	memmove(s, s + lw, len + 1 - lw);
	return s;
}
#undef __WHITESPACE

char *
strtrim(char *str)
{
	return strltrim(strrtrim(str));
}

const char *
strtail(const char *str, int len)
{
	return str + strlen(str) - len;
}

/*
 * Append a string to a dynamically-allocated string with length @len.
 * If the length of result string is greater than @len, this function
 * reallocates @dst and extend its size, and returns the new size.
 * If the reallocation fails, both @dst and @len are left unchanged,
 * and the function returns -1.
 */
ssize_t
strappend(char **dst, const char *src, size_t len)
{
	/* saves original @dst address */
	char *d = *dst;
	size_t len_s = strlen(src), len_d = strlen(d);
	if (len_d + len_s >= len) {
		if ((*dst = realloc(*dst, len + len_s)) == NULL) {
			*dst = d;
			return -1;
		}
		strlcat(*dst, src, len + len_s);
		return len + len_s;
	} else {
		strlcat(*dst, src, len);
		return len;
	}
}
