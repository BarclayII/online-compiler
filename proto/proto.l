%option noyywrap nodefault yylineno

%{
#include <stdio.h>
#include <string.h>

int lines = 0, cur_line = 0;
%}

%x CSOURCE
%x LEX
%x YACC
%x LINES
%x TEXT

NAME	([a-zA-Z0-9_\-\.]+)
PATH	([a-zA-Z0-9_\-\.]+)(\/[a-zA-Z0-9_\-\.]+)
CSOURCE	({NAME}\.c)
CHEADER	({NAME}\.h)
LEX	({NAME}\.l)
YACC	({NAME}\.y)
COBJ	({NAME}\.o)
TEXT	([ -~]*)
LINE	([ -~\t\r]*)
NUMBER	([0-9]+)

%%
USER" "{NAME}			{}
OUTEXEC" "{NAME}		{}
LIBS" "{TEXT}			{}
PKG-CONFIG" "{TEXT}		{}
CSOURCE" "{CSOURCE}		{}
<CSOURCE>CFLAGS" "{TEXT}	{}
CHEADER" "{CHEADER}		{}
LEX" "{LEX}			{}
<LEX>LEXOUT" "{CSOURCE}		{}
YACC" "{YACC}			{}
<YACC>YACCOUT" "{CSOURCE}	{}
<LINES>TEXTLINES" "{NUMBER}
<TEXT>{LINE}			{}
%%
