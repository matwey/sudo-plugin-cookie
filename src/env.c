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
size_t remove_env_by_key(const char* key, char** arge) {
	char** arge2;
	size_t keylen;
	keylen = strlen(key);
	for(arge2 = arge; *arge != NULL; ++arge) {
		if(strncmp(*arge, key, keylen) != 0) {
			*(arge2++) = *arge;
		}
	}
	*(arge2++) = NULL;
	return arge - arge2;
}
char** merge_env(char* const* arge, char* const* arge2) {
	size_t i = 0;
	size_t len = 0;
	char* const* tmp;
	char** ret;

	for(tmp = arge; *tmp != NULL; ++tmp,++len);
	for(tmp = arge2; *tmp != NULL; ++tmp,++len);

	ret = calloc(len+1, sizeof(char*));
	if(ret == NULL) {
		return ret;
	}

	for(; *arge != NULL; ++arge) {
		ret[i++] = *arge;
	}
	for(; *arge2 != NULL; ++arge2) {
		ret[i++] = *arge2;
	}
	ret[i++] = NULL;

	return ret;
}
