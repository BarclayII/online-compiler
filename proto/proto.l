%option noyywrap nodefault yylineno

%{
#include <stdio.h>
#include <string.h>
#include "proto.tab.h"
%}

%x TEXT

NAME	([a-zA-Z0-9_][a-zA-Z0-9_\-]*)
CSOURCE	{NAME}\.c
CHEADER	{NAME}\.h
LEX	{NAME}\.l
YACC	{NAME}\.y
COBJ	{NAME}\.o
TEXT	.*
TEXTNL	[.\n]*

%%
USER" "{NAME}		{ yylval.str = strdup(yytext + 5); return USER; }
CFLAGS" "{TEXT}		{ yylval.str = strdup(yytext + 7); return CFLAGS; }
LIBS" "{TEXT}		{ yylval.str = strdup(yytext + 5); return LIBS; }
PKG[\-_]CONFIG" "{TEXT}	{ yylval.str = strdup(yytext + 11);
			  return PKG_CONFIG; }
OUTEXEC" "{NAME}	{ yylval.str = strdup(yytext + 8); return OUTEXEC; }
OUTOBJ" "{COBJ}		{ yylval.str = strdup(yytext + 7); return OUTOBJ; }
%%
