#include "tuyalink_core.h"

#ifndef TUYA_CONNECT_H
#define TUYA_CONNECT_H

#define DATA_LEN 30

struct Device
{
  char productId[DATA_LEN];
  char deviceId[DATA_LEN];
  char deviceSecret[DATA_LEN];
};

void initID(char pId[], char dId[], char dSecret[]);
void tuya_connect(tuya_mqtt_context_t *client);
void send_memory_usage_to_tuya(tuya_mqtt_context_t *client, long int memory_usage);

#endif
