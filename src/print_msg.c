#include <err.h>
#include <stdlib.h>

#include "print_log.h"
#include "my_string.h"

char *get_printable_bitfield(char *payload, size_t len)
{
  char *res = malloc(sizeof(char) * len);
  if (!res)
    err(1, "cannot malloc in get_printable_bitfield");

  for (int i = 0; i < len; ++i)
    res[i] = payload[i];

  return res;
}

char *get_printable(char *payload)
{
  char 

char *get_printable_payload(char *type, char *payload, size_t len)
{
  if (strcmp(type, "bitfield") == 0)
    return get_printable_bitfield(payload, len);
  else if (strcmp(type, "have") == 0)
    return get_printable_bitfield(payload, 4);
  else if (strcmp(type, "request") == 0)
    return get_printable_request(payload);




void print_send_log(struct peers *p, char *type, char *payload, size_t len)
{
  char *msg1 = concat("send: ", p->url);
  char *msg2 = concat(msg1, ": ");
  free(msg1);
  char *msg3 = concat(msg2, type);
  free(msg2);
  
  if (payload)
  {
    char *msg4 = concat(msg3, " ");
    free(msg3);
    char *msg5 = concat(msg4, payload);
    free(msg4);
    print_log("msg", msg5);
    free(msg5);
  }
  else
  {
    print_log("msg", msgr3);
    free(msg3);
  }
}
