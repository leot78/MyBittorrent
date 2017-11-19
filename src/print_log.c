#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "print_log.h"

static struct log_info *logs;

void init_log(int active)
{
  logs = malloc(sizeof(struct log_info));
  if (!logs)
    err(1, "cannot malloc struct log_info");

  logs->torrent_id = NULL;
  logs->log_active = active;
}

int log_is_active(void)
{
  return logs->log_active;
}

void set_torrent_id(char *hash_str)
{
  logs->torrent_id = hash_str;
}

void print_log(char *action, char *msg)
{
  if (!logs->log_active)
    return;

  printf("%s: %s: %s\n", logs->torrent_id, action, msg);
}

void delete_log_info(void)
{
  free(logs->torrent_id);
  free(logs);
}
