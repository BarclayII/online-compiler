%option noyywrap nodefault yylineno reentrant

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "srvr_local.h"
#include "strl.h"
#include "lexer.h"

int lines = 0, cur_line = 0;

FILE *fp_source = NULL, *fp_source_conf = NULL, *fp_dir_conf = NULL;

char *tmpdir = NULL;
char *filename = NULL;

char *tmptxt = NULL;

%}

%x CSOURCE
%x LEX
%x YACC
%x LINES
%x TEXT

NAME	([a-zA-Z0-9_\-\.]+)
PATH	([a-zA-Z0-9_\-\.]+)(\/[a-zA-Z0-9_\-\.]+)
CSOURCE	({PATH}\.c)
CHEADER	({PATH}\.h)
LEX	({PATH}\.l)
YACC	({PATH}\.y)
COBJ	({PATH}\.o)
TEXT	([ -~]*)
LINE	([ -~\t\r]*)
NUMBER	([0-9]+)

%%
^USER" "{NAME}			{
	tmpdir = srvr_mkdir(tmptxt = strtrim(strdup(yyget_text(yyscanner) + 5)));
	if (tmpdir == NULL) {
		srvr_reply(sockfp, SR_ERROR_INTERN);
		srvr_yyfinish();
		return NULL;
	}
	free_n(&tmptxt);
}
^OUTEXEC" "{NAME}		{
	if (srvr_set_outexec(tmpdir, tmptxt = strtrim(strdup(yyget_text(yyscanner) + 8))) == NULL) {
		srvr_reply(sockfp, SR_ERROR_INTERN);
		srvr_yyfinish();
		return NULL;
	}
	free_n(&tmptxt);
}
^LDFLAGS" "{TEXT}		{
	if (srvr_set_ldflags(tmpdir, tmptxt = strtrim(strdup(yyget_text(yyscanner) + 8))) == NULL) {
		srvr_reply(sockfp, SR_ERROR_INTERN);
		srvr_yyfinish();
		return NULL;
	}
	free_n(&tmptxt);
}
^CSOURCE" "{CSOURCE}		{
	cur_file = srvr_create_file(
		tmpdir,
		filename = strtrim(strdup(yyget_text(yyscanner) + 8)));
	srvr_create_report_error();
	BEGIN CSOURCE;
}
<CSOURCE>^CFLAGS" "{TEXT}	{
	if (srvr_set_cflags(tmpdir, filename, 
	    tmptxt = strtrim(strdup(yyget_text(yyscanner) + 7))) == NULL) {
		srvr_reply(sockfp, SR_ERROR_INTERN);
		srvr_yyfinish();
		return NULL;
	}
	free_n(&tmptxt);
}
^CHEADER" "{CHEADER}		{
	cur_file = srvr_create_file(tmpdir,
		filename = strtrim(strdup(yyget_text(yyscanner) + 8)));
	srvr_create_report_error();
	BEGIN CSOURCE;
}
^LEX" "{LEX}			{
	cur_file = srvr_create_file(
		tmpdir,
		filename = strtrim(strdup(yyget_text(yyscanner) + 4)));
	srvr_create_report_error();
	BEGIN LEX;
}
<LEX>^LEXOUT" "{CSOURCE}		{
	if (srvr_set_lexout(tmpdir, filename, 
	    tmptxt = strtrim(strdup(yyget_text(yyscanner) + 7))) == NULL) {
		srvr_reply(sockfp, SR_ERROR_INTERN);
		srvr_yyfinish();
		return NULL;
	}
	free_n(&tmptxt);
}
^YACC" "{YACC}			{
	cur_file = srvr_create_file(
		tmpdir,
		filename = strtrim(strdup(yyget_text(yyscanner) + 5)));
	srvr_create_report_error();
	BEGIN YACC;
}
<YACC>^YACCOUT" "{CSOURCE}	{
	if (srvr_set_yaccout(tmpdir, filename, 
	    tmptxt = strtrim(strdup(yyget_text(yyscanner) + 8))) == NULL) {
		srvr_reply(sockfp, SR_ERROR_INTERN);
		srvr_yyfinish();
		return NULL;
	}
	free_n(&tmptxt);
}
<CSOURCE,LEX,YACC>^TEXTLINES" "{NUMBER}	{
	lines = atoi(yyget_text(yyscanner) + 10);
	cur_line = 0;
	if (lines > 0)
		BEGIN TEXT;
	else {
		if (fclose(cur_file) == EOF) {
			pinfo(PINFO_WARN, TRUE, "fclose");
		}
		free_n(&filename);
		BEGIN INITIAL;
	}
}
<TEXT>{LINE}			{
	if (srvr_puts(cur_file, yyget_text(yyscanner)) == NULL) {
		srvr_reply(sockfp, SR_ERROR_INTERN);
		srvr_yyfinish();
		return NULL;
	}
	free_n(&tmptxt);
	if (++cur_line == lines) {
		if (fclose(cur_file) == EOF) {
			pinfo(PINFO_WARN, TRUE, "fclose");
		}
		free_n(&filename);
		BEGIN INITIAL;
	}
}
END	{
	srvr_yyfinish();
	return 0;
}
.	{
	srvr_reply(sockfp, SR_ERROR_BAD_REQUEST);
	srvr_yyfinish();
	return NULL;
}
<<EOF>> {
	srvr_reply(sockfp, SR_ERROR_BAD_REQUEST);
	srvr_yyfinish();
	return NULL;
}
%%

void srvr_create_report_error(void)
{
	if (cur_file == NULL) {
		switch (errno) {
		case EEXIST:
		case EISDIR:
			srvr_reply(sockfp, SR_ERROR_DUPNAME);
			break;
		default:
			srvr_reply(sockfp, SR_ERROR_INTERN);
			break;
		}
		srvr_yyfinish();
		return NULL;
	}
}

void srvr_yyfinish(void)
{
	free_n(&tmptxt);
	free_n(&filename);
	return tmpdir;
}
