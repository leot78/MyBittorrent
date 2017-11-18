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

struct sockaddr_in *create_sock(struct raw_addr *ra)
{
  struct in_addr addr;
  addr.s_addr = ra->ip;
  
  struct sockaddr_in *sa = malloc(sizeof(struct sockaddr_in));
  if (!sa)
    err(1, "Could not allocate sockaddr_in struct");
  sa->sin_family = AF_INET;
  sa->sin_port = ra->port;
  sa->sin_addr = addr;
  return sa;
}

struct list *decode_bin(char *binaries)
{
  struct tracker *tr = parse_content(binaries);
  struct element *elt = get_value(tr->dict, "peers");
  char *start = elt->value;
  void *tmp = start;
  //get raw address
  struct raw_addr *ra = tmp;
  struct list *peer_list = init_list();
  for (unsigned i = 0; i < elt->size; i+=6, ra++)
    add_front(peer_list, create_sock(ra));
  delete_tracker(tr);
  free(binaries);
  return peer_list;
}

void free_sock_list(struct list *l_sa)
{
  while (l_sa->size)
   free(pop_front(l_sa));
  free(l_sa);
}
