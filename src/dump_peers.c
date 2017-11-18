#define _POSIX_C_SOURCE 201112L
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "my_bittorrent.h"

void decode_bin(char *binaries)
{
  binaries = binaries;
  struct sockaddr sa;
  char host[1024];
  char service[20];
  getnameinfo(&sa, sizeof(struct sockaddr), host, sizeof(host), service, sizeof(service), 0);
}

void dump_peers(struct tracker *tracker)
{
  unsigned char *hash = get_info_hash(tracker);
  char *urlp = get_tracker_url(tracker);
  char *tracker_response = get_tracker(urlp, hash);
  decode_bin(tracker_response);
  printf("raw tracker response :\n%s", tracker_response);
}
