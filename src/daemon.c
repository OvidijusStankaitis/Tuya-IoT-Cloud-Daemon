#include <assert.h>
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "tuyaConnect.h"
#include "argParser.h"

#include "tuyalink_core.h"
#include "getSysInfo.h"

#define DATA_LEN 30

tuya_mqtt_context_t client_instance;

int main(int argc, char **argv)
{
  Arguments args;
  parse_arguments(argc, argv, &args);

  printf("Product ID: %s\n", args.productId);
  printf("Device ID: %s\n", args.deviceId);
  printf("Device Secret: %s\n", args.deviceSecret);

  char productId[DATA_LEN];
  char deviceId[DATA_LEN];
  char deviceSecret[DATA_LEN];

  strcpy(productId, args.productId);
  strcpy(deviceId, args.deviceId);
  strcpy(deviceSecret, args.deviceSecret);

  initID(productId, deviceId, deviceSecret);
  tuya_mqtt_context_t *client = &client_instance;
  tuya_connect(client);

  for (;;)
  {
    tuya_mqtt_loop(client);
    long int memory_usage = get_memory_usage();
    send_memory_usage_to_tuya(client, memory_usage);
    sleep(10);
  }

  return 0;
}
