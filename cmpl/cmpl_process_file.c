
static char *cflags = NULL;
static char *ldflags = NULL;

/*
 */
void cmpl_process_file(char *filename)
{
	switch (getextension(filename)) {
	case FILE_CSOURCE:
		cmpl_process_csource(filename);
		break;
	case FILE_CHEADER:
		break;
	}
}
