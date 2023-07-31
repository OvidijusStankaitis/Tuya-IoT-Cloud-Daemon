#ifndef ARGPARSER_H
#define ARGPARSER_H

typedef struct Arguments {
    char* productId;
    char* deviceId;
    char* deviceSecret;
} Arguments;

void parse_arguments(int argc, char **argv, Arguments *args);

#endif