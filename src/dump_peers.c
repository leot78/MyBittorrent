#define _POSIX_C_SOURCE 201112L
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "my_bittorrent.h"
#include "parsing.h"

void decode_bin(char *binaries)
{
  struct tracker *tr = parse_content(binaries);
  struct element *elt = get_value(tr->dict, "peers");
  char *start = elt->value;
  void *tmp = start;
  //get raw address
  struct raw_addr *ra = tmp;
  ra = ra;

  struct in_addr addr;
  //convert ip to LE
  addr.s_addr= ntohl(ra->ip);
  
  //set sa value
  struct sockaddr_in sa;
  sa.sin_family = AF_INET;
  sa.sin_port = ra->port;
  sa.sin_addr = addr;
  
  char host[1024];
  char service[20];
  //get hostname
  getnameinfo((struct sockaddr*)&sa, sizeof(struct sockaddr), host,
              sizeof(host), service, sizeof(service), 0);
  char str[INET_ADDRSTRLEN];
  //get pretty print
  inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
}

void dump_peers(struct tracker *tracker)
{
  unsigned char *hash = get_info_hash(tracker);
  char *urlp = get_tracker_url(tracker);
  char *tracker_response = get_tracker(urlp, hash);
  decode_bin(tracker_response);
  printf("raw tracker response :\n%s", tracker_response);
}
