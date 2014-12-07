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
srvr_mkdir(const char *user, const char *dir, size_t size)
{
	char *dir = (char *)calloc(strlen(user) + 8);
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

	free_n(&path);

	return 0;
}

/*
 * Create OUTEXEC and LDFLAGS config files
 */
int
srvr_set_outexec(const char *dir, const char *outexec)
{
	return srvr_set_config(dir, OUTEXEC_CONFIG, outexec);
}

int
srvr_set_ldflags(const char *dir, const char *ldflags)
{
	return srvr_set_config(dir, LDFLAGS_CONFIG, ldflags);
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

	free_n(&path);

	return fp;
}

int
srvr_set_file_config(const char *dir, const char *file, const char *suffix,
    const char *content)
{
	int lf = strlen(file), lc = strlen(suffix);
	char *cflags_fname = (char *)calloc(sizeof(char), lf + lc + 1);
	memcpy(cflags_fname, file, lf + 1);
	memcpy(cflags_fname + lf, suffix, lc + 1);
	char *path = build_path(cflags_fname, dir);
	if (path == NULL) {
		pinfo(PINFO_ERROR, FALSE, "unable to build pathspec");
		return -1;
	}

	FILE *fp;
	if ((fp = fcreat(path, "w")) == NULL) {
		pinfo(PINFO_WARN, TRUE, "create %s failed", path);
		return -1;
	}

	if (fprintf(fp, "%s\n", content) < 0) {
		pinfo(PINFO_WARN, TRUE, "output content to %s failed", path);
		return -1;
	}

	if (fclose(fp) == EOF) {
		pinfo(PINFO_WARN, TRUE, "fclose %s", path);
		return -1;
	}

	free_n(&cflags_fname);
	free_n(&path);

	return 0;
}

int
srvr_set_cflags(const char *dir, const char *file, const char *cflags)
{
	return srvr_set_file_config(dir, file, CFLAGS_CONFIG, cflags);
}

int
srvr_set_lexout(const char *dir, const char *file, const char *lexout)
{
	return srvr_set_file_config(dir, file, LEXOUT_CONFIG, lexout);
}

int
srvr_set_yaccout(const char *dir, const char *file, const char *yaccout)
{
	return srvr_set_file_config(dir, file, YACCOUT_CONFIG, yaccout);
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
