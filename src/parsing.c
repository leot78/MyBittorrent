#include <stdio.h>
#include <stdlib.h>

size_t my_atoi(const char *str, char end, int *res)
{
  *res = 0;
  size_t i = 0;
  int sign = 1;
  
  if (str[i] == '-')
  {
    sign = -1;
    ++i;
  }

  while (str[i] != end)
  {
    *res = (*res * 10) + str[i] - '0';
    ++i;
  }

  *res *= sign;
  return i;
}

char *parse_string(const char *str, size_t *index)
{
  size_t len = 0;
  *index += my_atoi(str + *index, ':', &len) + 1;
  char *out = malloc(sizeof(char) * len + 1);
  out = memcpy(out, str + *index, len);
  out[len] = '\0';
  *index += len;
  return out;
}

int parse_number(const char *str, size_t *index)
{
  int res = 0;
  *index += my_atoi(str + *index, 'e', &res);
  return res;
}



int main(int argc, char **argv)
{
  FILE *file = fopen(argv[1], "r");

  fgetc(file)

  fclose(file);
}
