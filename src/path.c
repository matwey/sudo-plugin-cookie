#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "env.h"
#include "path.h"

char* resolve_path2(const char* file, const char* path) {
	char* path2;
	char* cpath;
	char* cfile;
	char* ptmp = NULL;

	path2 = strdup(path);
	for(cpath = path2; (cpath = strtok_r(cpath, ":", &ptmp)) != NULL; cpath += strlen(cpath) + 1) {
		if(asprintf(&cfile, "%s/%s", cpath, file) > 0) {
			if(access(cfile, X_OK | F_OK) == 0) {
				free(path2);
				return cfile;
			}
			free(cfile);
		}
	}

	free(path2);
	return NULL;
}

char* resolve_path(const char* file, char* const* arge) {
	char* path;
	path = find_env_by_key("PATH=", arge);

	if(strchr(file, '/') == NULL) {
		return resolve_path2(file, path);
	}
	if(access(file, X_OK | F_OK) == 0) {
		return strdup(file);
	}
	return NULL;
}
