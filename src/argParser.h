#ifndef ARGPARSER_H
#define ARGPARSER_H

#define DATA_LEN 30

typedef struct Arguments
{
    char *productId;
    char *deviceId;
    char *deviceSecret;
    int daemonize;
    int help;
} Arguments;

void parse_arguments(int argc, char **argv, Arguments *args);

#endif