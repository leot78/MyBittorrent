#include <stddef.h>
#include <stdint.h>

/**
** convert a uint32_t in an array of 4 bytes 
*/
char *uint32_to_char_net(char *res, uint32_t n)
{
  size_t shift = 8 * 3;
  for (size_t i = 0; i < 4; ++i)
  {
    res[i] = (n >> shift) & 0xFF;
    shift -= 8;
  }
  return res;
}
