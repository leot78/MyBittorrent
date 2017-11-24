#include <arpa/inet.h>
#include <err.h>
#include <stdlib.h>

#include "my_bittorrent.h"
#include "print_log.h"
#include "my_string.h"

unsigned int get_int(char *data, size_t index)
{
  return (data[index] << 24) | (data[index + 1] << 16)
          | (data[index + 2] << 8) | data[index + 3];
}

char *get_printable_bitfield(char *payload, size_t len)
{
  char *res = malloc(sizeof(char) * len + 1);
  if (!res)
    err(1, "cannot malloc in get_printable_bitfield");

  for (size_t i = 0; i < len; ++i)
    res[i] = payload[i] + '0';

  res[len] = 0;
  return res;
}

char *concat_request(char *res, struct raw_mess *rm)
{
  char *index = my_itoa(ntohl(rm->elt_1));
  char *begin = my_itoa(ntohl(rm->elt_2));
  char *len = my_itoa(ntohl(rm->elt_3));

  char *tmp1 = concat(res, index);
  free(res);
  char *tmp2 = concat(tmp1, " ");
  free(tmp1);
  tmp1 = concat(tmp2, begin);
  free(tmp2);
  tmp2 = concat(tmp1, " ");
  free(tmp1);
  tmp1 = concat(tmp2, len);
  free(tmp2);

  return tmp1;
}

char *concat_piece(char *res, struct raw_mess *rm)
{
  char *index = my_itoa(rm->elt_1);
  char *begin = my_itoa(rm->elt_2);

  char *tmp1 = concat(res, index);
  free(res);
  char *tmp2 = concat(tmp1, " ");
  free(tmp1);
  tmp1 = concat(tmp2, begin);
  free(tmp2);

  return tmp1;
}

void print_msg_log(struct peer *p, char *msg, char *mode_msg)
{
  char *msg1 = concat(mode_msg, p->url);
  char *msg2 = concat(msg1, ": ");
  free(msg1);

  char *res = NULL;
  char *tmp = NULL;
  if (msg[0] == 0x13)
    res = concat(msg2, "handshake");
  else
  {
    struct raw_mess *rm = (void *) msg;

    switch(rm->id)
    {
      case 0:
        res = concat(msg2, "choke");
        break;
      case 1:
        res = concat(msg2, "unchoke");
        break;
      case 2:
        res = concat(msg2, "interested");
        break;
      case 3:
        res = concat(msg2, "not interested");
        break;
      case 4:
        tmp = concat(msg2, "have ");
        char *index = my_itoa(ntohl(rm->elt_1));
        res = concat(tmp, index);
        free(tmp);
        free(index);
        break;
      case 5:
        tmp = concat(msg2, "bitfield ");
        char *bitfield = get_printable_bitfield(msg + 5, ntohl(rm->len) - 1);
        res = concat(tmp, bitfield);
        free(tmp);
        free(bitfield);
        break;
      case 6:
        tmp = concat(msg2, "request ");
        res = concat_request(tmp, rm);
        break;
      case 7:
        tmp = concat(msg2, "piece ");
        res = concat_piece(tmp, rm);
        break;
      default:
        res = concat(msg2, "unknown option");
        break;
    }
  }
  free(msg2);

  print_log("msg", res);
}
