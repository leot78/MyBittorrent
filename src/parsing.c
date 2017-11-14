#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"
#include "list/list.h"

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
  int len = 0;
  *index += my_atoi(str + *index, ':', &len) + 1;
  char *out = malloc(sizeof(char) * len + 1);
  out = memcpy(out, str + *index, len);
  out[len] = '\0';
  *index += len;
  return out;
}

char *parse_number(const char *str, size_t *index)
{
  *index += 1;
  size_t len = 0;
  while (str[len + *index] != 'e')
    len++;

  char *res = malloc(sizeof(char) * len + 1);
  res = memcpy(res, str + *index, len);
  *index += len + 1;
  res[len] = '\0';
  return res;
}

struct dictionary *parse_dict(const char *str, size_t *index)
{
  struct dictionary *dict = create_dict();

  while (str[*index] != 'e')
  {
    char *key = parse_string(str, index);
    char *value = NULL;

    if (str[*index] == 'i')
      value = parse_number(str, index);
    else
      value = parse_string(str, index);

    add_elt(dict, key, value, CHAR);
  }

  return dict;
}

void print_json_dict(struct dictionary *d, int pad)
{
  printf("%*s{\n", pad, "");
  struct node *cur = d->table->head;
  for (; cur; cur = cur->next)
  {
    struct element *elt = cur->data;
    printf("%*s\"%s\":", pad + 4, "", elt->key);
    char *value = elt->value;
    printf("\"%s\"\n", value);
  }
  printf("%*s}\n", pad, "");

}


int main(int argc, char **argv)
{
  argc = argc;
  argv = argv;

  size_t i = 1;
  struct dictionary *dict = parse_dict("d11:Hello Worldi1337e4:name7:Antoinee", &i);
  print_json_dict(dict, 0);
  delete_dict(dict);
}
