#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <unistd.h>

#include "tuyaConnect.h"
#include "argParser.h"
#include "daemon.h"
#include "getSysInfo.h"

#include "tuyalink_core.h"
#include "tuya_error_code.h"

#define DATA_LEN 30

tuya_mqtt_context_t client_instance;

int stop_loop = 0;

void handle_signal(int sig)
{
  if (sig == SIGINT || sig == SIGTERM || sig == SIGQUIT)
  {
    syslog(LOG_INFO, "Received signal to terminate. Disconnecting from Tuya...");
    stop_loop = 1;
  }
}

int main(int argc, char **argv)
{
  openlog("tuya_cloud_daemon", LOG_PID | LOG_CONS, LOG_USER);
  Arguments args;
  parse_arguments(argc, argv, &args);

  if (args.daemonize)
  {
    daemonize();
    syslog(LOG_INFO, "Daemon started");
  }

  tuya_mqtt_context_t *client = &client_instance;
  int ret = tuya_connect(client, args.deviceId, args.deviceSecret);

  switch (ret)
  {
    case 1:
      return 1;
      break;
    case -1:
      tuya_mqtt_deinit(client);
      return 1;
      break;
    case OPRT_OK:
      syslog(LOG_INFO, "Connected to Tuya");
      break;
    default:
      break;
  }

  while (!stop_loop)
  {
    tuya_mqtt_loop(&client_instance);
    long int memory_usage = get_memory_usage();
    if(memory_usage > 0) {
      send_memory_usage_to_tuya(client, memory_usage, args.deviceId);
      syslog(LOG_INFO, "Sent memory usage to Tuya: %ld", memory_usage);
    }
    sleep(1);
  }

  tuya_mqtt_disconnect(client);
  tuya_mqtt_deinit(client);
  syslog(LOG_INFO, "Disconnected from Tuya");

  closelog();
  return 0;
}