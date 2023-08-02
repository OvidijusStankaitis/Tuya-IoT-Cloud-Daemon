#include <argp.h>
#include <syslog.h>
#include <stdlib.h>

#include "argParser.h"

static struct argp_option options[] = {
    {"deviceId", 'd', "DEVICEID", 0, "Device ID"},
    {"productId", 'p', "PRODUCTID", 0, "Product ID"},
    {"deviceSecret", 's', "DEVICESECRET", 0, "Device Secret"},
    {"daemonize", 'D', NULL, 0, "Daemonize the process"},
    {"help", 'h', NULL, 0, "Help message"},
    {0}};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    Arguments *args = state->input;
    switch (key)
    {
    case 'd':
        args->deviceId = arg;
        break;
    case 'p':
        args->productId = arg;
        break;
    case 's':
        args->deviceSecret = arg;
        break;
    case 'D':
        args->daemonize = 1;
        break;
    case 'h':
        args->help = 1;
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, 0};

void parse_arguments(int argc, char **argv, Arguments *args)
{
    argp_parse(&argp, argc, argv, 0, 0, args);
    if (!args->deviceId || !args->productId || !args->deviceSecret)
    {
        if (args->help)
        {
            syslog(LOG_INFO, "Help message requested");
            printf("Device ID \t-d \t--deviceId\n");
            printf("Product ID \t-p \t--productId\n");
            printf("Device Secret \t-s \t--deviceSecret\n");
            printf("Daemonize \t-D \t--daemonize\n");
            printf("Help \t\t-h \t--help\n\n");
            printf("Usage: tuya_cloud_daemon -d <device_id> -p <product_id> -s <device_secret> -D\n");
            closelog();
            exit(EXIT_SUCCESS);
        }
        else
        {
            syslog(LOG_ERR, "Unspecified arguments");
            printf("tuya_cloud_daemon: too few arguments specified\n");
            printf("tuya_cloud_daemon: Use -h or --help for more information.\n");
            closelog();
            exit(EXIT_FAILURE);
        }
    }
    syslog(LOG_INFO, "Arguments provided");
}
