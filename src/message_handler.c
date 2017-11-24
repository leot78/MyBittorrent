#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>

#include "list/list.h"
#include "msg_creator.h"
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
  {
    //peer->have[i] = bitfield[i];
    peer->have[i] = (bitfield[i] & (1 << i)) >> i;
  }
}

void have_case(struct peer *peer, uint32_t id)
{
  peer->have[id] = 1;
  if (!g_client.have[id])
  {
    peer->client_interested = 1;
    send_simple_msg(peer, INTEREST);
  }
}

void piece_case(struct raw_mess *raw)
{
  raw->elt_1 = ntohl(raw->elt_1);
  raw->elt_2 = ntohl(raw->elt_2);
  raw->elt_3 = ntohl(raw->elt_3);
  if(g_client.requested != raw->elt_1)
    return;
  char *tmp = g_client.piece + raw->elt_2;
  tmp = memcpy(tmp, &raw->elt_3, raw->len - 8);

  //A vérifier.
  g_client.piece_len += raw->len - 8;
  g_client.requested = 0;
}

void make_request(struct list *peer_list)
{
  if (g_client.requested)
    return;
  size_t i = 0;
  struct node *cur = peer_list->head;
  for (; cur; cur = cur->next)
  {
    struct peer *peer = cur->data;
    for(; i < g_client.number_piece; i++)
    {
      if (peer->have[i] && !g_client.have[i] && !peer->client_choked
          && peer->client_interested)
      {
        size_t piece_len = g_client.piece_max_len;
        if (i == g_client.number_piece - 1)
        {
          struct dictionary *dict = get_value(g_client.tracker->dict, "info", 
                                              NULL);
          piece_len = get_len_from_files(dict) % g_client.piece_max_len;
        }
        piece_len = piece_len < MAX_PIECE_LEN ? piece_len : MAX_PIECE_LEN;
        send_request(peer, i, g_client.piece_len, piece_len);
        return;
      }
    }
  }
}

void make_all_handshake(struct list *peer_list)
{
  struct node *cur = peer_list->head;
  for (; cur; cur = cur->next)
  {
    struct peer *peer = cur->data;
    send_handshake(peer, g_client.tracker->info_hash, g_client.peer_id);
  }
}

void message_handler(char *message/*, size_t len*/, struct peer *peer,
                      struct list *peer_list)
{
  void *tmp = message;
  struct raw_mess *rm = tmp;
  rm->len = ntohl(rm->len);
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
      bitfield_case(peer, message + 5, rm->len);
      break;
      //bitfield
    case 6:
      break;
      //request
    case 7:
      piece_case(rm);
      break;
      //piece
    default:
      return;
  }
  make_request(peer_list);
}
