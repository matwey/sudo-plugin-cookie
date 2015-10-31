#include <stdlib.h>
#include <string.h>

#include "env.h"

char* find_env_by_key(const char* key, char* const* arge) {
	size_t keylen;
	keylen = strlen(key);
	for(; *arge != NULL; ++arge) {
		if(strncmp(*arge, key, keylen) != 0) {
			continue;
		}
		return *arge + keylen;
	}
	return NULL;
}
