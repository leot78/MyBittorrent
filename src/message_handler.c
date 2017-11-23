#include <arpa/inet.h>
#include <stdint.h>

#include "my_bittorrent.h"
#include "my_string.h"

struct client g_client;

/*
{
  peer->client_choked = 1;
}

void unchoke_case(struct peer *peer)
{
  peer->client_choked = 0;
}

void interested_case(struct peer *peer)
{
  peer->peer_interested = 1;
}

void not_interested_case(struct peer *peer)
{
  peer->peer_interested = 0;
}
*/
void bitfield_case(struct peer *peer, char *bitfield, size_t len)
{
  size_t bitfield_len = len - 1;
  for (unsigned i = 0; i < bitfield_len; i++) 
    peer->have[i] = (i & (1 << i)) >> i;
}

void have_case(struct peer *peer, unsigned id)
{
  peer->have[id] = 1;
  if (!client[id])
  {
    peer->client_interested = 1;
    send_interested(peer);
  }
}

void piece_case(struct peer *peer, struct raw_mess raw)
{
  if(g_client.requested != raw.elt)
    return;
  void *tmp = g_client.piece + raw.begin;
  tmp = memcpy(g_client.piece + raw.begin, raw.elt_3, raw.len - 8);
  //Je suis débile casting void* essayer &machin
}

char *message_handler(char *message, size_t len, struct peer *peer)
{
  void *tmp = message;
  struct raw_mess *rm = tmp;
  rm->len = ntohl(rm->len);
  rm->id = ntohl(rm->id);
  if (!len_m)
    return NULL;
  switch(rm->id)
  {
    case 0:
      //choke
    case 1:
      //unchoke
    case 2:
      //interested
    case 3:
      //not interested
    case 4:
      //have
    case 5:
      //bitfield
    case 6:
      //request
    case 7:
      //piece
    case 8:
      //cancel
    default:
      //
  }
  len = len;
  return message;
}
