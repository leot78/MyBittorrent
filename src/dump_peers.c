#define _POSIX_C_SOURCE 201112L
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "dictionary.h"
#include "list/list.h"
#include "my_bittorrent.h"
#include "parsing.h"
#include "print_log.h"

void get_peers_url(struct sockaddr_in *sa, char *host, char *service)
{
  getnameinfo((struct sockaddr*)sa, sizeof(struct sockaddr), host,
              HOST_LEN, service, SERVICE_LEN, 0);
}


void print_peers(struct list *peer_list)
{
  struct node *it = peer_list->head;
  for (; it; it = it->next)
  {
    char host[HOST_LEN];
    char service[SERVICE_LEN];
    struct peer *peer = it->data;
    get_peers_url(peer->sa, host, service);
    peer->service = service;
    peer->host = host;
    printf("%s:%s\n", host, service);
  }
}

struct list *get_peers(struct tracker *tracker)
{
  unsigned char *hash = get_info_hash(tracker);
  set_torrent_id(get_hash(hash, 3));
  char *urlp = get_tracker_url(tracker);
  struct dictionary *dict = get_value(tracker->dict, "info", NULL);
  int file_len = atoi(get_value(dict, "length", NULL));
  int piece_len = atoi(get_value(dict, "piece length", NULL));
  char *tracker_response = get_tracker(urlp, hash);
  free(hash);
  return decode_bin(tracker_response, file_len / piece_len);
  //printf("raw tracker response :\n%s", tracker_response);
}
