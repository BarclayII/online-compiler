#ifndef _WARN_H
#define _WARN_H

#include <stdarg.h>

void pinfo(int level, const char *fmt, ...);

#define WARNLVL_DEBUG	1	/* garbage debug information */
#define WARNLVL_INFO	2	/* somehow important info */
#define WARNLVL_WARNING	3	/* warning, caution needed */
#define WARNLVL_ERROR	4	/* error occured but not fatal */
#define WARNLVL_FATAL	5	/* fatal error which may terminate processes */
#define WARNLVL_PANIC	6	/* abrupt panic which should kill immediately */

#endif
