
static char *ldflags = NULL;

void cmpl_process_file(char *filename)
{
	switch (getextension(filename)) {
	case FILE_CSOURCE:
		cmpl_process_csource(filename);
		break;
	case FILE_CHEADER:
		break;
	case FILE_LEX:
		cmpl_process_lex(filename);
		break;
	case FILE_YACC:
		cmpl_process_yacc(filename);
		break;
	default:
		break;
	}
}

void cmpl_process_init(void)
{
	ldflags = (char *)malloc(BUFSIZ);
}

void cmpl_process_finalize(void)
{
	free_n(&ldflags);
}
