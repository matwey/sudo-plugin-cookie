#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sudo_plugin.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define COOKIE_FILE "cookie.txt"

struct policy_plugin_state {
	sudo_printf_t plugin_printf;
	char* const* plugin_arge;
};

struct policy_plugin_state state;

char* load_cookie_from_file(const char* filename) {
	char* buf = NULL;
	char* retbuf = NULL;
	struct stat st;
	int ret,fd;
	size_t i,j;

	ret = access(filename, R_OK | F_OK);
	if(ret < 0) {
		state.plugin_printf(SUDO_CONV_ERROR_MSG, "Cannot access cookie file: %s\n", strerror(errno));
		goto end;
	}

	ret = stat(filename, &st);
	if(ret < 0) {
		state.plugin_printf(SUDO_CONV_ERROR_MSG, "Cannot stat cookie file: %s\n", strerror(errno));
		goto end;
	}
	if(st.st_uid != 0) {
		state.plugin_printf(SUDO_CONV_ERROR_MSG, "Cookie file is not owned by uid=0\n");
		goto end;
	}
	if(st.st_mode & 0777 != 0600) {
		state.plugin_printf(SUDO_CONV_ERROR_MSG, "Cookie file has wrong access permissions %o\n", st.st_mode);
		goto end;
	}
	if(st.st_size == 0) {
		state.plugin_printf(SUDO_CONV_ERROR_MSG, "Cookie file is empty\n");
		goto end;
	}

	fd = open(filename, O_RDONLY);
	if(fd < 0) {
		state.plugin_printf(SUDO_CONV_ERROR_MSG, "Cannot open cookie file: %s\n", strerror(errno));
		goto end;
	}

	buf = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if(buf == MAP_FAILED) {
		state.plugin_printf(SUDO_CONV_ERROR_MSG, "Cannot read cookie file: %s\n", strerror(errno));
		goto unmap_end;
	}

	retbuf = (char*)calloc(st.st_size+1, sizeof(char));
	if(retbuf == NULL) {
		state.plugin_printf(SUDO_CONV_ERROR_MSG, "Cannot allocate memory: %s\n", strerror(errno));
	}

	for(i = 0, j = 0; i < st.st_size; ++i) {
		if(!isspace(buf[i])) {
			retbuf[j++] = buf[i];
		}
	}
	retbuf[j] = 0;

	munmap(buf, st.st_size);
unmap_end:
	close(fd);
end:
	return retbuf;
}
char* load_cookie() {
	return load_cookie_from_file(COOKIE_FILE);
}
char* find_cookie_in_env(char* env_add[]) {
	char* ret;
	const char cookie_key[] = "SUDO_COOKIE";

	for(; *env_add != NULL; ++env_add) {
		if(strncmp(*env_add, cookie_key, sizeof(cookie_key)-1) != 0) {
			continue;
		}
		ret = strchr(*env_add, '=');
		if(ret != NULL) {
			return ret+1;
		}
		env_add++;
	}
	return NULL;
}

char ** build_command_info(const char *command) {
	char **command_info;
	int i = 0;

	command_info = calloc(1, sizeof(char *));
	if (command_info == NULL)
		return NULL;
	asprintf(&command_info[i++], "command=%s", command);
	return command_info;
}

int policy_open(unsigned int version, sudo_conv_t conversation, sudo_printf_t plugin_printf, char * const settings[], char * const user_info[], char * const user_env[], char * const plugin_options[]) {
	state.plugin_printf = plugin_printf;

	if (SUDO_API_VERSION_GET_MAJOR(version) != SUDO_API_VERSION_MAJOR) {
		plugin_printf(SUDO_CONV_ERROR_MSG, "Cookie plugin requires API version %d.x (%d.%d is available)\n",
			SUDO_API_VERSION_MAJOR,
			SUDO_API_VERSION_GET_MAJOR(version),
			SUDO_API_VERSION_GET_MINOR(version));
		return -1;
	}

	state.plugin_arge = user_env;

	return 1;
}
void policy_close(int exit_status, int error) {
}
int policy_version(int verbose) {
	state.plugin_printf(SUDO_CONV_INFO_MSG, "Cookie policy plugin version %d\n", 1);
	return 0;
}
int policy_check(int argc, char * const argv[], char *env_add[], char **command_info[], char **argv_out[], char **user_env_out[]) {
	char* cookie;
	char* env_cookie;

	cookie = load_cookie();
	if(cookie == NULL)
		return -1;

	state.plugin_printf(SUDO_CONV_INFO_MSG, "Loaded cookie is %s\n", cookie);

	env_cookie = find_cookie_in_env(env_add);
	if(env_cookie == NULL) {
		free(cookie);
		return 0;
	}

	state.plugin_printf(SUDO_CONV_INFO_MSG, "Supplied cookie is %s\n", env_cookie);

	if(strcmp(env_cookie, cookie) != 0) {
		free(cookie);
		return 0;
	}

	*command_info = build_command_info(argv[0]);
	*argv_out =  argv;
	*user_env_out = state.plugin_arge;

	free(cookie);
	return 1;
}
int policy_list(int verbose, const char *list_user, int argc, char * const argv[]) {
	return 0;
}

struct policy_plugin cookie_policy = {
	SUDO_POLICY_PLUGIN,
	SUDO_API_VERSION,
	policy_open,
	policy_close,
	policy_version,
	policy_check,
	policy_list,
	NULL, /* validate */
	NULL, /* invalidate */
	NULL, /* init_session */
	NULL, /* register_hooks */
	NULL /* deregister_hooks */
};
