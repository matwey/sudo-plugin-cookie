#include <stdlib.h>
#include <sudo_plugin.h>

struct policy_plugin_state {
	sudo_printf_t plugin_printf;
};

struct policy_plugin_state state;

int policy_open(unsigned int version, sudo_conv_t conversation, sudo_printf_t plugin_printf, char * const settings[], char * const user_info[], char * const user_env[], char * const plugin_options[]) {
	state.plugin_printf = plugin_printf;
	return 1;
}
void policy_close(int exit_status, int error) {
}
int policy_version(int verbose) {
	state.plugin_printf(SUDO_CONV_INFO_MSG, "Cookie policy plugin version %d\n", 1);
	return 0;
}
int policy_check(int argc, char * const argv[], char *env_add[], char **command_info[], char **argv_out[], char **user_env_out[]) {
	return 0;
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
