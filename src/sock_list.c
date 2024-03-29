#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "list/list.h"
#include "my_bittorrent.h"
#include "my_string.h"
#include "parsing.h"

/**
** Initialize a #peer with given parameters.
*/
struct peer *init_peer(int nb_pieces, struct sockaddr_in *sa)
{
  struct peer *peer = malloc(sizeof(struct peer));
  if (!peer)
    err(1, "Could not allocate peer struct");
  peer->sa = sa;
  int *have = calloc(nb_pieces, sizeof(int));
  if (!have)
    err(1, "Could not allocate have array");
  peer->have = have;
  peer->client_interested = 0;
  peer->peer_interested = 0;
  peer->client_choked = 1;
  peer->peer_choked = 1;
  peer->handshaked = 0;
  peer->nb_pieces = nb_pieces;
  peer->url = NULL;
  peer->q_send = init_list();
  peer->socket = -1;
  return peer;
}

/**
** Create sockaddress and peer, return peer.
*/
struct peer *create_sock(struct raw_addr *ra, int nb_pieces)
{
  struct in_addr addr;
  addr.s_addr = ra->ip;
  
  struct sockaddr_in *sa = malloc(sizeof(struct sockaddr_in));
  if (!sa)
    err(1, "Could not allocate sockaddr_in struct");
  sa->sin_family = AF_INET;
  sa->sin_port = ra->port;
  sa->sin_addr = addr;
  struct peer *peer = init_peer(nb_pieces, sa);
  if (!peer)
    err(1, "Could not allocate peer struct");
  return peer;
}

/**
** Create peer_list from binaries received by GET request.
*/
struct list *decode_bin(char *binaries, int nb_pieces)
{
  struct tracker *tr = parse_content(binaries);
  tr->info_hash = NULL;
  size_t size = 0;
  struct raw_addr *ra = get_value(tr->dict, "peers", &size);
  struct list *peer_list = init_list();
  for (unsigned i = 0; i < size; i+=6, ra++)
    add_front(peer_list, create_sock(ra, nb_pieces));
  delete_tracker(tr);
  free(binaries);
  return peer_list;
}

/**
** Free peer list.
*/
void free_sock_list(struct list *l_sa)
{
  while (l_sa->size)
  {
    struct peer *peer = pop_front(l_sa);
    free_peer(peer);
  }
  free(l_sa);
}

/**
** Free given peer.
*/
void free_peer(struct peer *peer)
{
  if (peer->socket != -1)
    close(peer->socket);
  free_list(peer->q_send);
  free(peer->sa);
  free(peer->have);
  free(peer->url);
  free(peer);
}
