#include <stdlib.h>
#include <string.h>

char *my_itoa(long value)
{
  int pos = 0;
  unsigned len = 1;
  int signe = 0;
  if (value < 0)
  {
    value = value * -1;
    signe = 1;
    len++;
  }
  long save = value;
  for (;value >= 10; value /= 10)
    len++;
  char *res = malloc((len + 1) * sizeof(char));
  if (signe)
    res[0] = '-';
  
  res[len] = '\0';
  for (unsigned i = len - 1;i != 0; i--, pos++, save /= 10)
    res[i] = save % 10 + '0';
  
  res[signe] = value + '0';
  return res;
}

char *concat(char *c1, char *c2)
{
  char *res = malloc((strlen(c1) + strlen(c2) + 1) * sizeof(char));
  if (!res)
    return NULL;
  strcpy(res, c1);
  strcat(res, c2);
  return res;
}

char *concatn(char *c1, char *c2, size_t l1, size_t l2)
{
  char *res = malloc((l1 + l2) * sizeof(char));
  if (!res)
    return NULL;
  memcpy(res, c1, l1);
  memcpy(res + l1, c2, l2);
  return res;
}

char *strcpy_delim(char *dest, char *src, char delim)
{
  size_t i = 0;
  for (; src[i] != delim; i++)
    dest[i] = src[i];
  dest[i] = '\0';
  return dest;
}
