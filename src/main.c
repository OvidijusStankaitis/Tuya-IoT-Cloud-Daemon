#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <setjmp.h>

#include "tuyaConnect.h"
#include "argParser.h"
#include "daemon.h"
#include "getSysInfo.h"

#include "tuyalink_core.h"
#include "tuya_error_code.h"

#define DATA_LEN 30

tuya_mqtt_context_t client_instance;
tuya_mqtt_context_t *client = &client_instance;
static jmp_buf jmpbuf;
static bool run = false;

static void handle_signal(int sig)
{
	if (sig == SIGINT || sig == SIGTERM || sig == SIGQUIT)
	{
		if (run)
		{
			syslog(LOG_INFO, "Received signal to terminate. Disconnecting from Tuya...");
			siglongjmp(jmpbuf, 1);
		}

		exit(0);
	}
}

int main(int argc, char **argv)
{
	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);
	signal(SIGQUIT, handle_signal);

	openlog("tuya_cloud_daemon", LOG_PID | LOG_CONS, LOG_USER);
	Arguments args;
	parse_arguments(argc, argv, &args);

	if (args.daemonize)
	{
		daemonize();
		syslog(LOG_INFO, "Daemon started");
	}

	int ret = tuya_connect(client, args.deviceId, args.deviceSecret);

	if (ret)
	{
		goto cleanup;
	}

	run = true;

	while (run)
	{
		if (sigsetjmp(jmpbuf, 1))
		{
			run = false;
			goto cleanup;
		}

		tuya_mqtt_loop(client);
		double memory_usage = get_memory_usage();
		if (memory_usage > 0)
		{
			send_memory_usage_to_tuya(client, memory_usage, args.deviceId);
			syslog(LOG_INFO, "Sent memory usage to Tuya: %0.2f GB", memory_usage);
		}
		sleep(0);
	}

cleanup:
	tuya_mqtt_disconnect(client);
	tuya_mqtt_deinit(client);
	syslog(LOG_INFO, "Disconnected from Tuya");
	closelog();
	return 0;
}