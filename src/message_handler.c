#include <arpa/inet.h>
#include <stdint.h>

#include "my_bittorrent.h"
#include "my_string.h"

struct client g_client;

char *generate_handshake(unsigned char *info_hash)
{
  char *tmp = (char*)info_hash;
  char *len = "\x013";
  char *startshake = concatn(len, "BitTorrent protocol\0\0\0\0\0\0\0\0", 1,
                             27);
  char *handshake = concatn(startshake, tmp, 28, 20);
  return handshake;
}

char *choke_case(struct peer *peer)
{
  peer->client_choked = 1;
  return NULL;
}

char *unchoke_case(struct peer *peer)
{
  peer->client_choked = 0;
  return NULL;
}

char *interested_case(struct peer *peer)
{
  peer->peer_interested = 1;
  return NULL;
}

char *not_interested_case(struct peer *peer)
{
  peer->peer_interested = 0;
  return NULL;
}

char *bitfield_case(struct peer *peer, struct client *client, char *bitfield, 
                    size_t len)
{
  size_t bitfield_len = len - 1;
  for (unsigned i = 0; i < bitfield_len; i++) 
    peer->have[i] = (i & (1 << i)) >> i;
  return NULL;
}

char *have_case(struct peer *peer, struct client *client, unsigned id)
{
  peer->have[id] = 1;
  if (!client[id])
  {
    peer->client_interested = 1;
    uint32_t len = 1;
    char mess[4];
    char *len_mess = to_char_net(len, 4);
    char *id_mess = to_char_net(2, 1);
    char *res = concatn(len_mess, id_mess, 4, 1);
    free(len_mess);
    free(id_mess);
    return res;
  }
  return NULL;
}

char *piece_case(struct peer *peer, struct client *client, 

char *message_handler(char *message, size_t len, struct peer *peer,
                      struct client *client)
{
  static int defined = 0;
  if (!defined)
    g_client = init_clien(
  void *tmp = message;
  struct raw_mess *rm= tmp;
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
