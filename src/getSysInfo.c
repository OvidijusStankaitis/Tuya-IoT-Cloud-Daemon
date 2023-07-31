#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "getSysInfo.h"

long int get_memory_usage()
{
    openlog("getSysInfo", LOG_PID|LOG_CONS, LOG_USER);
    FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL)
    {
        syslog(LOG_ERR, "Error opening /proc/meminfo");
        return -1;
    }

    int total_memory;
    int free_memory;
    int buffers;
    int cached;

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line, "MemTotal: %d kB", &total_memory))
        {
            continue;
        }
        else if (sscanf(line, "MemFree: %d kB", &free_memory))
        {
            continue;
        }
        else if (sscanf(line, "Buffers: %d kB", &buffers))
        {
            continue;
        }
        else if (sscanf(line, "Cached: %d kB", &cached))
        {
            continue;
        }
    }

    fclose(file);

    long int used_memory = (total_memory - free_memory - buffers - cached) * 1024;
    syslog(LOG_INFO, "Memory usage calculated: %ld", used_memory);
    closelog();
    return used_memory;
}