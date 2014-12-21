#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strl.h"
#include "srvr_local.h"

/*
 * Create a temporary directory according to user name
 * Returns name of the created directory
 */
char *
srvr_mkdir(const char *user)
{
	int size = strlen(user) + 8;
	char *dir = (char *)calloc(size);
	strlcpy(dir, user, size);
	strlcat(dir, ".XXXXXX", size);
	
	if (mkdtemp(dir) == NULL) {
		pinfo(PINFO_WARN, TRUE, "mkdir for %s", user);
		return NULL;
	}
	pinfo(PINFO_DEBUG, "mkdir %s", dir);

	return dir;
}

/*
 * Create a configuration file under the specified directory for different
 * specifications
 */
static int
srvr_set_config(const char *dir, const char *file, const char *content)
{
	char *path = build_path(file, dir);
	if (path == NULL) {
		pinfo(PINFO_ERROR, FALSE, "unable to build pathspec");
		return -1;
	}

	FILE *fp;
	if ((fp = fopen(path, "w")) == NULL) {
		pinfo(PINFO_WARN, TRUE, "fopen %s", path);
		return -1;
	}
	pinfo(PINFO_DEBUG, FALSE, "fopen %s", path);

	if (fprintf(fp, "%s\n", content) < 0) {
		pinfo(PINFO_WARN, TRUE, "fprintf %s", path);
		return -1;
	}
	pinfo(PINFO_DEBUG, FALSE, "fprintf %s %s", path, content);

	if (fclose(fp) == EOF) {
		pinfo(PINFO_WARN, TRUE, "fclose %s", path);
		return -1;
	}
	pinfo(PINFO_DEBUG, FALSE, "fclose %s", path);

	free_n(path);

	return 0;
}

/*
 * Create OUTEXEC config files
 */
int
srvr_set_outexec(const char *dir, const char *outexec)
{
	return srvr_set_config(dir, OUTEXEC_CONFIG, outexec);
}

FILE *
srvr_create_file(const char *dir, const char *file)
{
	FILE *fp;
	char *path = build_path(file, dir);
	if (path == NULL) {
		pinfo(PINFO_ERROR, FALSE, "unable to build pathspec");
		return NULL;
	}

	/* check if the file already exists */
	if ((fp = fcreat(path, "w")) == NULL) {
		switch (errno) {
		case EEXIST:
			pinfo(PINFO_WARN, FALSE,
			    "file %s requested already exists", path);
			break;
		case EISDIR:
			pinfo(PINFO_WARN, FALSE,
			    "file %s is actually a directory", path);
			break;
		default:
			break;
		}
	}

	free_n(path);

	return fp;
}

int
srvr_puts(const FILE *fp, const char *line)
{
	if (fputs(line, fp) == EOF) {
		pinfo(PINFO_WARN, TRUE, "fputs");
		return -1;
	}
	return 0;
}
