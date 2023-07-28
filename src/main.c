#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "tuyaConnect.h"
#include "daemon.h"
#include "argParser.h"

#include "cJSON.h"

#define DEV_INFO_FILE 16

tuya_mqtt_context_t client_instance;

int main(int argc, char **argv)
{
  char deviceFile[DEV_INFO_FILE];
  strncpy(deviceFile, argv[1], DEV_INFO_FILE - 1);
  deviceFile[DEV_INFO_FILE - 1] = '\0';
  printf("%s\n", deviceFile);

  FILE *deviceInfo = fopen(deviceFile, "r");

  if (deviceInfo == NULL)
  {
    syslog(LOG_ERR, "Error opening file %s", deviceFile);
  }

  // Read the file into a string
  fseek(deviceInfo, 0, SEEK_END);
  long length = ftell(deviceInfo);
  fseek(deviceInfo, 0, SEEK_SET);
  char *data = malloc(length + 1);
  if (data)
  {
      fread(data, 1, length, deviceInfo);
  }
  fclose(deviceInfo);
  data[length] = '\0'; // Null-terminate the string

  // Parse the JSON
  cJSON *json = cJSON_Parse(data);
  if (json == NULL)
  {
      const char *error_ptr = cJSON_GetErrorPtr();
      if (error_ptr != NULL)
      {
          fprintf(stderr, "Error before: %s\n", error_ptr);
      }
      return 1;
  }

  // Get the values
  cJSON *productId = cJSON_GetObjectItemCaseSensitive(json, "productId");
  cJSON *deviceId = cJSON_GetObjectItemCaseSensitive(json, "deviceId");
  cJSON *deviceSecret = cJSON_GetObjectItemCaseSensitive(json, "deviceSecret");

  if (cJSON_IsString(productId) && (productId->valuestring != NULL))
  {
      printf("productId: %s\n", productId->valuestring);
  }

  if (cJSON_IsString(deviceId) && (deviceId->valuestring != NULL))
  {
      printf("deviceId: %s\n", deviceId->valuestring);
  }

  if (cJSON_IsString(deviceSecret) && (deviceSecret->valuestring != NULL))
  {
      printf("deviceSecret: %s\n", deviceSecret->valuestring);
  }

  // Clean up
  cJSON_Delete(json);
  free(data);

  tuya_mqtt_context_t *client = &client_instance;
  tuya_connect(client);

  for (;;)
  {
    tuya_mqtt_loop(client);
  }

  return 0;
}
