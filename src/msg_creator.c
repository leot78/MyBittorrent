#include <stdlib.h>

#include "list/list.h"
#include "my_bittorrent.h"
#include "print_log.h"
#include "int_utils.h"
#include "my_string.h"
#include "msg_creator.h"

/**
** generate the handshake
*/
char *generate_handshake(unsigned char *info_hash, char *peer_id)
{
  char *tmp = (char*)info_hash;
  char *len = "\x013";
  char *startshake = concatn(len, "BitTorrent protocol\0\0\0\0\0\0\0\0", 1,
                             27);
  if (!tmp)
    return startshake;
  char *harlemshake = concatn(startshake, tmp, 28, 20);
  free(startshake);
  char *handshake = concatn(harlemshake, peer_id, 48, 20);
  free(harlemshake);

  return handshake;
}

/**
** send an handshake to a peer
*/
void send_handshake(struct peer *p, unsigned char *info_hash, char *peer_id)
{
  char *handshake = generate_handshake(info_hash, peer_id);
  add_tail(p->q_send, handshake);
}

/**
** send a message without payload
*/
void send_simple_msg(struct peer *p, enum type_simple_msg type)
{
  char *str = calloc(1, 5);
  str[4] = type;
  str = uint32_to_char_net(str, 1);
  add_tail(p->q_send, str);
}

/**
** send a have message
*/
void send_have(struct peer *p, size_t piece_index)
{
  char *str = calloc(1, 9);
  str[4] = 4;
  str = uint32_to_char_net(str, 5);
  uint32_to_char_net(str + 5, piece_index);
  add_tail(p->q_send, str);
}

/**
** send a bitfield message
*/
void send_bitfield(struct peer *p, size_t len, char *bitfield)
{
  char *str = calloc(1, 5 + len);
  str[4] = 5;
  str = uint32_to_char_net(str, 1 + len);
  char *res = concatn(str, bitfield, 5, len);
  free(str);
  add_tail(p->q_send, res);
}

/**
** send a request message
*/
void send_request(struct peer *p, size_t index, size_t begin, size_t length)
{
  char *str = calloc(1, 17);
  str[4] = 6;
  str = uint32_to_char_net(str, 13);

  uint32_to_char_net(str + 5, index);
  uint32_to_char_net(str + 9, begin);
  uint32_to_char_net(str + 13, length);

  add_tail(p->q_send, str);
}

/**
** send a piece message
*/
void send_piece(struct peer *p, size_t index, size_t begin,
                struct block *block)
{
  char *str = calloc(1, 13 + block->size);
  str[4] = 7;
  str = uint32_to_char_net(str, 9 + block->size);

  uint32_to_char_net(str + 5, index);
  uint32_to_char_net(str + 9, begin);

  char *res = concatn(str, block->data, 13, block->size);
  free(str);

  add_tail(p->q_send, res);
}
