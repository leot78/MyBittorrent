#include <stdlib.h>

#include "my_bittorrent.h"
#include "print_log.h"
#include "int_utils.h"
#include "my_string.h"

char *generate_handshake(unsigned char *info_hash)
{
  char *tmp = (char*)info_hash;
  char *len = "\x013";
  char *startshake = concatn(len, "BitTorrent protocol\0\0\0\0\0\0\0\0", 1,
                             27);
  char *handshake = concatn(startshake, tmp, 28, 20);
  return handshake;
}

void send_handshake(struct peer *p, unsigned char *info_hash)
{
  char *handshake = generate_handshake(info_hash);
  p->to_send = handshake;
}

void send_simple_msg(struct peer *p, enum type_simple_msg type)
{
  char *str = calloc(1, 5);
  str[4] = type;
  str = uint32_to_char_net(str, 1);
  p->to_send = str;
}

void send_have(struct peer *p, size_t piece_index)
{
  char *str = calloc(1, 9);
  str[4] = 4;
  str = uint32_to_char_net(str, 5);
  uint32_to_char_net(str + 5, piece_index);
  p->to_send = str;
}

void send_bitfield(struct peer *p, size_t len, char *bitfield)
{
  char *str = calloc(1, 5 + len);
  str[4] = 5;
  str = uint32_to_char_net(str, 1 + len);
  char *res = concatn(str, bitfield, 5, len);
  free(str);
  free(bitfield);
  p->to_send = res;
}

void send_request(struct peer *p, size_t index, size_t begin, size_t length)
{
  char *str = calloc(1, 17);
  str[4] = 6;
  str = uint32_to_char_net(str, 13);
  
  uint32_to_char_net(str + 5, index);
  uint32_to_char_net(str + 9, begin);
  uint32_to_char_net(str + 13, length);

  p->to_send = str;
}

void send_piece(struct peer *p, size_t index, size_t begin, char *block)
{
  char *str
