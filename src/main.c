#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "tuyaConnect.h"
#include "argParser.h"
#include "daemon.h"

#include "tuyalink_core.h"
#include "getSysInfo.h"

#define DATA_LEN 30

tuya_mqtt_context_t client_instance;

int main(int argc, char **argv)
{
  openlog("Connetion", LOG_PID | LOG_CONS, LOG_USER);
  syslog(LOG_INFO, "Parsing arguments..");

  Arguments args;
  parse_arguments(argc, argv, &args);

  char productId[DATA_LEN];
  char deviceId[DATA_LEN];
  char deviceSecret[DATA_LEN];

  strcpy(productId, args.productId);
  strcpy(deviceId, args.deviceId);
  strcpy(deviceSecret, args.deviceSecret);

  daemonize();
  initID(productId, deviceId, deviceSecret);
  tuya_mqtt_context_t *client = &client_instance;
  tuya_connect(client);

  for (;;)
  {
    tuya_mqtt_loop(client);
    long int memory_usage = get_memory_usage();
    send_memory_usage_to_tuya(client, memory_usage);
    syslog(LOG_INFO, "Sent memory usage to Tuya: %ld", memory_usage);
    sleep(10);
  }

  closelog();

  return 0;
}