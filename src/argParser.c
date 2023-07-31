#include <argp.h>

#include "argParser.h"

const char *argp_program_version = "tuya_daemon 1.0";
const char *argp_program_bug_address = "<bugreport@yourdomain.com>";

static char doc[] = "Your Program Description";

static struct argp_option options[] = {
    {"deviceId", 'd', "DEVICEID", 0, "Device ID"},
    {"productId", 'p', "PRODUCTID", 0, "Product ID"},
    {"deviceSecret", 's', "DEVICESECRET", 0, "Device Secret"},
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
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, 0, doc};

void parse_arguments(int argc, char **argv, Arguments *args)
{
    argp_parse(&argp, argc, argv, 0, 0, args);
}
