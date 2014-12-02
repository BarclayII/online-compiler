#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strl.h"
#include "srvr_local.h"

/*
 * Create a temporary directory according to user name
 * Returns name of the created directory
 */
char *srvr_mkdir(const char *user, const char *dir, size_t size)
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
static int srvr_set_config(const char *dir, const char *file, const char *content)
{
	/* dir/.outexec */
	int path_len = strlen(dir) + strlen(file) + 1;
	char *path = (char *)calloc(path_len + 1);
	if (path == NULL) {
		pinfo(PINFO_WARN, TRUE, "calloc");
		return -1;
	}

	strlcpy(path, dir, path_len + 1);
	strlcat(path, PATH_SEP, path_len + 1);
	strlcat(path, file, path_len + 1);

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
int srvr_set_outexec(const char *dir, const char *outexec)
{
	return srvr_set_config(dir, OUTEXEC_CONFIG, outexec);
}

int srvr_set_ldflags(const char *dir, const char *ldflags)
{
	return srvr_set_config(dir, LDFLAGS_CONFIG, ldflags);
}
