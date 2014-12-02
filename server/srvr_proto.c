#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strl.h"
#include "srvr_local.h"

/*
 * Create a temporary directory according to user name
 * Returns name of the created directory
 */
char *srvr_mkdir(const char *user)
{
	char *dir = (char *)calloc(strlen(user) + 8);
	strlcpy(dir, user);
	strlcat(dir, ".XXXXXX");
	
	if (mkdtemp(dir) == NULL) {
		pinfo(PINFO_WARN, TRUE, "mkdir for %s", user);
		return NULL;
	}
	pinfo(PINFO_DEBUG, "mkdir %s", dir);

	return dir;
}

/*
 * Create a ".outexec" configuration file under the specified directory for
 * output executable specification
 */
int srvr_set_outexec(const char *dir, const char *outexec)
{
	if (chdir(dir) == -1) {
		pinfo(PINFO_WARN, TRUE, "chdir to %s", dir);
		return -1;
	}
	pinfo(PINFO_DEBUG, FALSE, "chdir to %s", dir);

	FILE *fp;
	if ((fp = fopen(OUTEXEC_CONFIG, "w")) == NULL) {
		pinfo(PINFO_WARN, TRUE, "fopen %s", OUTEXEC_CONFIG);
		return -1;
	}
	pinfo(PINFO_DEBUG, FALSE, "fopen %s", OUTEXEC_CONFIG);

	if (fprintf(fp, "%s\n", outexec) < 0) {
		pinfo(PINFO_WARN, TRUE, "fprintf %s", OUTEXEC_CONFIG);
		return -1;
	}
	pinfo(PINFO_DEBUG, FALSE, "fprintf %s %s", OUTEXEC_CONFIG, outexec);

	if (fclose(fp) == EOF) {
		pinfo(PINFO_WARN, TRUE, "fclose %s", OUTEXEC_CONFIG);
		return -1;
	}
	pinfo(PINFO_DEBUG, FALSE, "fclose %s", OUTEXEC_CONFIG);

	if (chdir("..") == -1) {
		pinfo(PINFO_WARN, TRUE, "return to parent directory failed");
		return -1;
	}
	pinfo(PINFO_DEBUG, FALSE, "return to parent directory");

	return 0;
}
