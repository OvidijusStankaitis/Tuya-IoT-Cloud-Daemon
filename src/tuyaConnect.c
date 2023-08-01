#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>

#include "tuya_error_code.h"
#include "system_interface.h"
#include "mqtt_client_interface.h"
#include "tuyalink_core.h"
#include "tuya_cacert.h"

#include "tuyaConnect.h"

void outToFile(char *str)
{
    FILE *fp;
    fp = fopen("tuyaConnect.log", "a+");
    fseek(fp, 0, SEEK_END);
    fprintf(fp, "%s\n", str);
    fclose(fp);
}

char* parse_string(const char *json_string) {
    cJSON *json_data = cJSON_Parse(json_string);
    cJSON *inputParams = cJSON_GetObjectItemCaseSensitive(json_data, "inputParams");
    cJSON *string_item = cJSON_GetObjectItemCaseSensitive(inputParams, "string");
    char *result = strdup(string_item->valuestring);
    cJSON_Delete(json_data);
    return result;
}

void on_connected(tuya_mqtt_context_t *context, void *user_data)
{
    syslog(LOG_INFO, "Connected");
}

void on_disconnect(tuya_mqtt_context_t *context, void *user_data)
{
    syslog(LOG_INFO, "Disconnected");
}

void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg)
{
    syslog(LOG_INFO, "on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
    switch (msg->type)
    {
    case THING_TYPE_DEVICE_SUB_BIND_RSP:
        syslog(LOG_INFO, "bind response:%s\r\n", msg->data_string);
        break;

    case THING_TYPE_DEVICE_TOPO_GET_RSP:
        syslog(LOG_INFO, "get topo response:%s\r\n", msg->data_string);
        break;
    case THING_TYPE_ACTION_EXECUTE:
        syslog(LOG_INFO, "action execute:%s\r\n", msg->data_string);
        // outToFile(msg->data_string);
        char *result = parse_string(msg->data_string);
        outToFile(result);
        break;
    default:
        break;
    }
    printf("\r\n");
}

void tuya_connect(tuya_mqtt_context_t *client, char dId[], char dSecret[])
{
    int ret = OPRT_OK;

    ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t){
                                     .host = "m1.tuyacn.com",
                                     .port = 8883,
                                     .cacert = tuya_cacert_pem,
                                     .cacert_len = sizeof(tuya_cacert_pem),
                                     .device_id = dId,
                                     .device_secret = dSecret,
                                     .keepalive = 100,
                                     .timeout_ms = 2000,
                                     .on_connected = on_connected,
                                     .on_disconnect = on_disconnect,
                                     .on_messages = on_messages});
    syslog(LOG_INFO, "tuya_mqtt_init return status: %d", ret);

    ret = tuya_mqtt_connect(client);
    syslog(LOG_INFO, "tuya_mqtt_connect return status: %d", ret);
}

void send_memory_usage_to_tuya(tuya_mqtt_context_t *client, long int memory_usage, char deviceId[])
{
    char data[256];
    snprintf(data, sizeof(data), "{\"MemoryUsage\":{\"value\":%ld}}", memory_usage);

    tuyalink_thing_property_report(client, deviceId, data);
    closelog();
}