#ifndef _ENV_H
#define _ENV_H

char* find_env_by_key(const char* key, char* const* arge);
char** merge_env(char* const* arge, char* const* arge2);
size_t remove_env_by_key(const char* key, char** arge);

#endif // _ENV_H
