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
  tmp = memcpy(g_client.piece + raw.begin, &raw.elt_3, raw.len - 8);
  //A vérifier.
  g_client.requested = 0;
}

void make_request(struct peer *peer, struct raw_mess raw
                  struct list *peer_list)
{
  if (g_client.requested)
    return;
  unsigned i = 0;
  for(; i < peer_list->size; i++)
  {
    if (peer_list->have[i] && !g_client.have[i] && !peer->client_choked
        && peer->client_interested)
    {
      0

char *message_handler(char *message/*, size_t len*/, struct peer *peer,
                      struct list *peer_list)
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
      peer->client_choked = 1;
      break;
      //choke
    case 1:
      peer->client_choked = 0;
      break;
      //unchoke
    case 2:
      peer->peer_interested = 1;
      break;
      //interested
    case 3:
      peer->peer_interested = 0;
      break;
      //not interested
    case 4:
      have_case(peer, rm->elt_1);
      break;
      //have
    case 5:
      bitfield_case(peer, rm->elt_1, rm->len);
      break;
      //bitfield
    case 6:
      break;
      //request
    case 7:
      piece_case(peer, rm);
      //piece
    default:
      //
  }
  len = len;
  return message;
}
