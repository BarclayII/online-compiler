%option noyywrap nodefault yylineno

%{
#include <stdio.h>
#include <string.h>

int lines = 0, cur_line = 0;

FILE *fp_source = NULL, *fp_source_conf = NULL, *fp_dir_conf = NULL;

char *user = NULL, *tmpdir = NULL;
char *outexec = NULL;
char *ldflags = NULL;
char *filename = NULL;
char *conf = NULL;
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
^USER" "{NAME}			{
	tmpdir = srvr_mkdir(user = strdup(yytext + 5));
}
^OUTEXEC" "{NAME}		{
	srvr_set_outexec(tmpdir, outexec = strdup(yytext + 8);
}
^LDFLAGS" "{TEXT}		{
	srvr_set_ldflags(tmpdir, ldflags = strdup(yytext + 8));
}
^CSOURCE" "{CSOURCE}		{
	cur_file = srvr_open_file(tmpdir, filename = strdup(yytext + 8), "w");
	BEGIN CSOURCE;
}
<CSOURCE>^CFLAGS" "{TEXT}	{
	srvr_set_cflags(tmpdir, filename, cflags = strdup(yytext + 7));
}
^CHEADER" "{CHEADER}		{
	cur_file = srvr_open_file(tmpdir, filename = strdup(yytext + 8), "w");
}
^LEX" "{LEX}			{
	cur_file = srvr_open_file(tmpdir, filename = strdup(yytext + 4), "w");
	BEGIN LEX;
}
<LEX>^LEXOUT" "{CSOURCE}		{
	srvr_set_lexout(tmpdir, filename, conf = strdup(yytext + 7));
}
^YACC" "{YACC}			{
	cur_file = srvr_open_file(tmpdir, filename = strdup(yytext + 5), "w");
	BEGIN YACC;
}
<YACC>^YACCOUT" "{CSOURCE}	{
	srvr_set_yaccout(tmpdir, filename, conf = strdup(yytext + 8));
}
<CSOURCE,LEX,YACC>^TEXTLINES" "{NUMBER}	{
	lines = atoi(yytext + 10);
	cur_line = 0;
	if (lines > 0)
		BEGIN TEXT;
	else
		BEGIN INITIAL;
}
<TEXT>{LINE}			{
	srvr_puts(cur_file, yytext);
	if (++cur_line == lines)
		BEGIN INITIAL;
}
%%

char *srvr_mkdir(const char *user)
{
	/* dir = user.XXXXXX */
	char *dir = (char *)calloc(strlen(user) + 8, sizeof(char));
	strlcpy(dir, user);
	strlcat(dir, ".XXXXXX");
	if (mkdtemp(dir) == NULL)
		return NULL;
	if (chdir(dir) == -1) {
		rmdir(dir);
		return NULL;
	}
	if ((fp_dir_conf = fopen(".conf", "w")) == NULL) {
		rmdir(dir);
		return NULL;
	}
}
