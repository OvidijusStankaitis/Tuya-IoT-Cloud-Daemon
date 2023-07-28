#ifndef TUYA_CONNECT_H
#define TUYA_CONNECT_H

#include <assert.h>
#include <stdio.h>
#include <syslog.h>
// #include "tuya_log.h"
#include "tuya_error_code.h"
#include "system_interface.h"
#include "mqtt_client_interface.h"
#include "tuyalink_core.h"

#define DATA_LEN 30

struct Device
{
  char productId[DATA_LEN];
  char deviceId[DATA_LEN];
  char deviceSecret[DATA_LEN];
};

void tuya_connect(tuya_mqtt_context_t *client);

#endif // TUYA_CONNECT_H
