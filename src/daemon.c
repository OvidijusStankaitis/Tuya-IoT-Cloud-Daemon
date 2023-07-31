#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include "tuyaConnect.h"
#include "argParser.h"

#include "tuyalink_core.h"
#include "getSysInfo.h"

#define DATA_LEN 30

tuya_mqtt_context_t client_instance;

void daemonize();

int main(int argc, char **argv)
{
  openlog("Daemon", LOG_PID | LOG_CONS, LOG_USER);
  syslog(LOG_INFO, "Parsing arguments..");

  Arguments args;
  parse_arguments(argc, argv, &args);

  char productId[DATA_LEN];
  char deviceId[DATA_LEN];
  char deviceSecret[DATA_LEN];

  strcpy(productId, args.productId);
  strcpy(deviceId, args.deviceId);
  strcpy(deviceSecret, args.deviceSecret);

  syslog(LOG_INFO, "Starting daemon...");

  daemonize();
  initID(productId, deviceId, deviceSecret);
  tuya_mqtt_context_t *client = &client_instance;
  tuya_connect(client);

  syslog(LOG_INFO, "Daemon started.");

  for (;;)
  {
    tuya_mqtt_loop(client);
    long int memory_usage = get_memory_usage();
    send_memory_usage_to_tuya(client, memory_usage);
    syslog(LOG_INFO, "Sent memory usage to Tuya: %ld", memory_usage);
    sleep(10);
  }

  closelog();

  return 0;
}

void daemonize()
{
  pid_t pid;

  pid = fork();

  if(pid < 0)
  {
    exit(EXIT_FAILURE);
  }

  if(pid > 0)
  {
    exit(EXIT_SUCCESS);
  }

  if(setsid() < 0)
  {
    exit(EXIT_FAILURE);
  }

  signal(SIGCHLD, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  pid = fork();

  if(pid < 0)
  {
    exit(EXIT_FAILURE);
  }

  if(pid > 0)
  {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  if (chdir("/") != 0) 
  {
    syslog(LOG_ERR, "Changing directory to root failed");
    exit(EXIT_FAILURE);
  }

  int x;
  for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
  {
      close(x);
  }
}