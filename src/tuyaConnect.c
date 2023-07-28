#include "tuyaConnect.h"
#include "tuya_cacert.h"

const char productId[] = "agcfuzi9mnras1gs";
const char deviceId[] = "262e7e1c9162ca6b27kvcl";
const char deviceSecret[] = "AkCmJ4AXgdgHdi8Y";

void on_connected(tuya_mqtt_context_t* context, void* user_data)
{
  syslog(LOG_INFO, "Connected");
}

void on_disconnect(tuya_mqtt_context_t* context, void* user_data)
{
    syslog(LOG_INFO, "Disconnected");
}

void on_messages(tuya_mqtt_context_t* context, void* user_data, const tuyalink_message_t* msg)
{
    syslog(LOG_INFO, "on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
    switch (msg->type) {
        case THING_TYPE_DEVICE_SUB_BIND_RSP:
      syslog(LOG_INFO, "bind response:%s\r\n", msg->data_string);
            break;

        case THING_TYPE_DEVICE_TOPO_GET_RSP:
            syslog(LOG_INFO, "get topo response:%s\r\n", msg->data_string);
            break;

        default:
            break;
    }
    printf("\r\n");
}

void tuya_connect(tuya_mqtt_context_t* client)
{
    int ret = OPRT_OK;

    ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t) {
        .host = "m1.tuyacn.com",
        .port = 8883,
        .cacert = tuya_cacert_pem,
        .cacert_len = sizeof(tuya_cacert_pem),
        .device_id = deviceId,
        .device_secret = deviceSecret,
        .keepalive = 100,
        .timeout_ms = 2000,
        .on_connected = on_connected,
        .on_disconnect = on_disconnect,
        .on_messages = on_messages
    });
    assert(ret == OPRT_OK);

    ret = tuya_mqtt_connect(client);
    assert(ret == OPRT_OK);
}