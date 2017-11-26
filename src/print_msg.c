#include <arpa/inet.h>
#include <err.h>
#include <stdlib.h>

#include "my_bittorrent.h"
#include "print_log.h"
#include "my_string.h"

/**
** get an unsigned from an array of byte
*/
unsigned int get_int(char *data, size_t index)
{
  return (data[index] << 24) | (data[index + 1] << 16)
          | (data[index + 2] << 8) | data[index + 3];
}

/**
** get a string of a bitfield
*/
char *get_printable_bitfield(char *payload, size_t len)
{
  char *res = malloc(sizeof(char) * g_client.number_piece + 1);
  if (!res)
    err(1, "cannot malloc in get_printable_bitfield");

  size_t cpt = 0;
  for (size_t j = 0; j < len; ++j)
  {
    uint8_t byte = payload[j];
    for (int i = 7; i >= 0; i--)
    {
      res[cpt] = ((byte >> i) & 0x01) + '0';
      cpt++;
      if (cpt == g_client.number_piece)
      {
        res[g_client.number_piece] = 0;
        return res;
      }
    }
  }
  return res;
}

/**
** build a request message
*/
char *concat_request(char *msg2, struct raw_mess *rm)
{
  char *tmp = concat(msg2, "request ");
  char *index = my_itoa(ntohl(rm->elt_1));
  char *begin = my_itoa(ntohl(rm->elt_2));
  char *len = my_itoa(ntohl(rm->elt_3));

  char *tmp1 = concat(tmp, index);
  free(tmp);
  char *tmp2 = concat(tmp1, " ");
  free(tmp1);
  tmp1 = concat(tmp2, begin);
  free(tmp2);
  tmp2 = concat(tmp1, " ");
  free(tmp1);
  tmp1 = concat(tmp2, len);
  free(tmp2);

  free(index);
  free(begin);
  free(len);

  return tmp1;
}


/**
** build a piece message
*/
char *concat_piece(char *msg2, struct raw_mess *rm)
{
  char *tmp = concat(msg2, "piece ");
  char *index = my_itoa(ntohl(rm->elt_1));
  char *begin = my_itoa(ntohl(rm->elt_2));

  char *tmp1 = concat(tmp, index);
  free(tmp);
  char *tmp2 = concat(tmp1, " ");
  free(tmp1);
  tmp1 = concat(tmp2, begin);
  free(tmp2);

  free(index);
  free(begin);
  return tmp1;
}

/**
** build a have message
*/
char *concat_have(char *msg2, struct raw_mess *rm)
{
  char *tmp = concat(msg2, "have ");
  char *index = my_itoa(ntohl(rm->elt_1));
  char *res = concat(tmp, index);
  free(tmp);
  free(index);
  return res;
}

/**
** build a bitfield message
*/
char *concat_bitfield(char *msg, char *msg2, struct raw_mess *rm)
{
  char *tmp = concat(msg2, "bitfield ");
  char *bitfield = get_printable_bitfield(msg + 5, ntohl(rm->len) - 1);
  char *res = concat(tmp, bitfield);
  free(tmp);
  free(bitfield);
  return res;
}

/**
** build the correct message
*/
char *get_message(char *msg, char *msg2)
{
  char *res = NULL;
  struct raw_mess *rm = (void *) msg;
  if (msg[0] == 0x13)
    res = concat(msg2, "handshake");
  else if (ntohl(rm->len) <= 0)
    free(msg2);
  else if (rm->id == 0)
    res = concat(msg2, "choke");
  else if (rm->id == 1)
    res = concat(msg2, "unchoke");
  else if (rm->id == 2)
    res = concat(msg2, "interested");
  else if (rm->id == 3)
    res = concat(msg2, "not interested");
  else if (rm->id == 4)
    res = concat_have(msg2, rm);
  else if (rm->id == 5)
    res = concat_bitfield(msg, msg2, rm);
  else if (rm->id == 6)
    res = concat_request(msg2, rm);
  else if (rm->id == 7)
    res = concat_piece(msg2, rm);
  else
    res = concat(msg2, "unknown option");
  return res;
}

/**
** print the correct message log
*/
void print_msg_log(struct peer *p, char *msg, char *mode_msg)
{
  if (!log_is_active())
    return;

  char *msg1 = concat(mode_msg, p->url);
  char *msg2 = concat(msg1, ": ");
  free(msg1);
  char *res = get_message(msg, msg2); 

  free(msg2);
  print_log("msg", res);
  free(res);
}
