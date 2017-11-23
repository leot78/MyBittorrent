#include <arpa/inet.h>
#include <err.h>

char *uint32_to_char_net(char *res, uint32_t n)
{
  n = htonl(n);
  size_t shift = 8 * 3;
  for (size_t i = 0; i < 4; ++i)
  {
    res[i] = (n >> shift) & 0xFF;
    shift -= 8;
  }
  return res;
}

char *uint8_to_char_net(uint8_t n)
{
  n = htonl(n);
  char *res = malloc(sizeof(char));
  if (!res)
    err(1, "cannot malloc in to_char_net");

  size_t shift = 8 * 3;
  for (size_t i = 0; i < 4; ++i)
  {
    res[i] = (n >> shift) & 0xFF;
    shift -= 8;
  }
  return res;
}

uint32_t to_uint32_host(char *data, size_t index)
{
  size_t shift = 8 * 3;
  uint32_t res = data[index] << shift;
  for (size_t i = 1; i < len; ++i)
  {
    res |= data[index + i] << shift;
    shift -= 8;
  }

  return ntohl(res);
}
