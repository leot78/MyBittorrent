#define _POSIX_C_SOURCE 201112L
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "list/list.h"
#include "my_bittorrent.h"
#include "parsing.h"
#include "print_log.h"

void print_peers(struct list *peer_list)
{
  //struct sockaddr_in *sa;
  struct node *it = peer_list->head;
  for (; it; it = it->next)
  {
    char host[1024];
    char service[20];
    //get hostname
    getnameinfo((struct sockaddr*)it->data, sizeof(struct sockaddr), host,
              sizeof(host), service, sizeof(service), 0);
    //char str[INET_ADDRSTRLEN];
    //get pretty print
    //inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
    printf("%s:%s\n", host, service);
  }
}

struct list *get_peers(struct tracker *tracker)
{
  unsigned char *hash = get_info_hash(tracker);
  set_torrent_id(get_hash(hash, 3));
  char *urlp = get_tracker_url(tracker);
  struct dictionary *dict = get_value(tracker->dict, "info");
  int file_len = atoi(get_value(dict, "length"));
  int piece_len = atoi(get_value(dict, "piece length"));
  char *tracker_response = get_tracker(urlp, hash);
  free(hash);
  return decode_bin(tracker_response, file_len / piece_len);
  //printf("raw tracker response :\n%s", tracker_response);
}
