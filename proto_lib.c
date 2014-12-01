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

	return dir;
}
