#ifndef _PINFO_H
#define _PINFO_H

#include <stdarg.h>

#include "bool.h"

void pinfo(int level, bool append, const char *fmt, ...);
void vpinfo(int level, bool append, const char *fmt, va_list vp);

#define PINFO_DEBUG	1	/* garbage debug information */
#define PINFO_INFO	2	/* somehow important info */
#define PINFO_WARN	3	/* warning, caution needed */
#define PINFO_ERROR	4	/* error occured but not fatal */
#define PINFO_FATAL	5	/* fatal error which may terminate processes */
#define PINFO_PANIC	6	/* abrupt panic which should kill immediately */

#endif
