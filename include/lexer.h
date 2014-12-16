#ifndef _LEXER_H
#define _LEXER_H

#include <stdio.h>

#define YY_DECL	int yylex(FILE *fp, yyscan_t yyscanner)

#define YY_INPUT(buf, result, max_size) \
	do { \
		int c = '*'; \
		size_t n; \
		for ( n = 0; n < max_size && \
			     (c = getc( yyin )) != EOF && c != '\n'; ++n ) \
			buf[n] = (char) c; \
		if ( c == '\n' ) \
			buf[n++] = (char) c; \
		if ( c == EOF && ferror( yyin ) ) \
			YY_FATAL_ERROR( "input in flex scanner failed" ); \
		result = n; \
	} while (0)

#endif
