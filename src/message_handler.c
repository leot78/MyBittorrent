#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>

#include "list/list.h"
#include "msg_creator.h"
#include "my_bittorrent.h"
#include "my_string.h"

struct client g_client;

/**
** Handle reception of bitfield message.
** Set the have array according to bitfield.
*/
void bitfield_case(struct peer *peer, char *bitfield, size_t len)
{
  size_t bitfield_len = len - 1;
  size_t cpt = 0;
  for (unsigned j = 0; j < bitfield_len; j++)
  {
    for (int i = 7; i >= 0; --i)
    {
      peer->have[cpt] = (bitfield[j] & (1 << i)) >> i;
      if (peer->have[cpt] && !g_client.have[cpt] && !peer->client_interested)
      {
        peer->client_interested = 1;
        send_simple_msg(peer, INTEREST);
      }
      cpt++;
      if (cpt == g_client.number_piece)
        return;
    }
  }
}

/**
** Handle reception of have message.
** Set have and send INTERESTED if needed.
*/
void have_case(struct peer *peer, uint32_t id)
{
  peer->have[id] = 1;
  if (!g_client.have[id] && !peer->client_interested)
  {
    peer->client_interested = 1;
    send_simple_msg(peer, INTEREST);
  }
}

/**
** Check if have_list is full.
*/
void check_have(void)
{
  unsigned i = 0;
  while (i < g_client.number_piece && g_client.have[i])
    i++;
  g_client.finish = (i == g_client.number_piece);
}

/**
** Handle piece message.
** Append block to buff, if full piece write to file and call for next
*/
void piece_case(struct raw_mess *raw)
{
  raw->elt_1 = ntohl(raw->elt_1);
  raw->elt_2 = ntohl(raw->elt_2);
  if(!g_client.requested)
    return;
  char *tmp = g_client.piece + raw->elt_2;
  tmp = memcpy(tmp, &raw->elt_3, raw->len - 9);

  g_client.piece_len += raw->len - 9;
  g_client.requested = 0;
  size_t piece_len = g_client.piece_max_len;
  if (raw->elt_1 == g_client.number_piece - 1)
  {
    struct dictionary *dict = get_value(g_client.tracker->dict, "info", NULL);
    piece_len = get_len_from_files(dict) % g_client.piece_max_len;
  }
  if (g_client.piece_len == piece_len)
  { 
    if (check_piece(g_client.piece, g_client.piece_len, raw->elt_1))
    {
      write_piece(raw->elt_1);
      g_client.have[raw->elt_1] = 1;
      check_have();
    }
    g_client.request_id = -1;
    g_client.piece_len = 0;
  }
}

/**
** Find peer with with given piece.
*/
void verify_have(size_t i, struct peer *peer)
{
  size_t piece_len = g_client.piece_max_len;
  if (i == g_client.number_piece - 1)
  {
    struct dictionary *dict = get_value(g_client.tracker->dict, "info",
        NULL);
    piece_len = (get_len_from_files(dict) % g_client.piece_max_len)
                - g_client.piece_len;
  }
  piece_len = piece_len < MAX_PIECE_LEN ? piece_len : MAX_PIECE_LEN;
  send_request(peer, i, g_client.piece_len, piece_len);
  g_client.requested = 1;
  g_client.request_id = i;
  return;
}

/**
** Find peer with any piece not already downloaded.
*/
void find_peer(struct node *cur)
{
  for (; cur; cur = cur->next)
  {
    struct peer *peer = cur->data;
    if(!peer->client_interested)
      continue;
    for(size_t i = 0; i < g_client.number_piece; i++)
    {
      if (peer->have[i] && !g_client.have[i] && !peer->client_choked)
      {
        verify_have(i, peer);
        return;
      }
    }
  }
}

/**
** Find a peer open to request with a piece not downloaded.
*/
void make_request(struct list *peer_list)
{
  if (g_client.requested)
    return;
  struct node *cur = peer_list->head;
  if (g_client.request_id == -1)
    find_peer(cur);
  else
  {
    size_t i = g_client.request_id;
    for (; cur; cur = cur->next)
    {
      struct peer *peer = cur->data;
      if(!peer->client_interested)
        continue;
      if (peer->have[i] && !g_client.have[i] && !peer->client_choked)
      {
        verify_have(i, peer);
        return;
      }
    }
  }
}

/**
** Send handshake to every peer.
*/
void make_all_handshake(struct list *peer_list)
{
  struct node *cur = peer_list->head;
  for (; cur; cur = cur->next)
  {
    struct peer *peer = cur->data;
    send_handshake(peer, g_client.tracker->info_hash, g_client.peer_id);
  }
}

/**
** Handle reception and answer from peers.
*/
void message_handler(char *message, struct peer *peer,
                      struct list *peer_list)
{
  if (message[0] == 0x13)
  {
    peer->handshaked = 1;
    return;
  }
  if (!peer->handshaked)
    return;
  void *tmp = message;
  struct raw_mess *rm = tmp;
  rm->len = ntohl(rm->len);
  if (rm->id == 0)
    peer->client_choked = 1;
  else if (rm->id == 1)
    peer->client_choked = 0;
  else if (rm->id == 2)
    peer->peer_interested = 1;
  else if (rm->id == 3)
    peer->peer_interested = 1;
  else if (rm->id == 4)
    have_case(peer, ntohl(rm->elt_1));
  else if (rm->id == 5)
    bitfield_case(peer, message + 5, rm->len);
  else if (rm->id == 7)
    piece_case(rm);
  make_request(peer_list);
}
