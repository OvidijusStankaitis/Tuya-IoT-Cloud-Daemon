#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <unistd.h>

#include "tuyaConnect.h"
#include "argParser.h"
#include "daemon.h"

#include "tuyalink_core.h"
#include "getSysInfo.h"

#define DATA_LEN 30

tuya_mqtt_context_t client_instance;

int stop_loop = 0;

void handle_signal(int sig) {
    if (sig == SIGINT || sig == SIGTERM || sig == SIGQUIT) {
        syslog(LOG_INFO, "Received signal to terminate. Disconnecting from Tuya...");
        stop_loop = 1;
    }
}

int main(int argc, char **argv)
{
  signal(SIGINT, handle_signal);
  signal(SIGTERM, handle_signal);
  signal(SIGQUIT, handle_signal);

  openlog("Connetion", LOG_PID | LOG_CONS, LOG_USER);
  syslog(LOG_INFO, "Parsing arguments..");

  Arguments args;
  parse_arguments(argc, argv, &args);

  char productId[DATA_LEN];
  char deviceId[DATA_LEN];
  char deviceSecret[DATA_LEN];
  char daemon[DATA_LEN];
  strcpy(productId, args.productId);
  strcpy(deviceId, args.deviceId);
  strcpy(deviceSecret, args.deviceSecret);
  strcpy(daemon, args.daemonize);

  printf("daemon: %s\n", daemon);

  if (!strcmp(daemon, "yes"))
  {
    syslog(LOG_INFO, "Daemonizing process...");
    daemonize();
  }

  tuya_mqtt_context_t *client = &client_instance;
  tuya_connect(client, deviceId, deviceSecret);

  while (!stop_loop)
  {
    tuya_mqtt_loop(&client_instance);
    long int memory_usage = get_memory_usage();
    send_memory_usage_to_tuya(client, memory_usage, deviceId);
    syslog(LOG_INFO, "Sent memory usage to Tuya: %ld", memory_usage);
    sleep(5);
  }

  tuya_mqtt_disconnect(client);
  tuya_mqtt_deinit(client);
  syslog(LOG_INFO, "Disconnected from Tuya. Exiting...");

  closelog();
  return 0;
}