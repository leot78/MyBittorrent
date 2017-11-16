# define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_bittorrent.h"
#include "parsing.h"
#include "dictionary.h"
#include "list/list.h"

size_t my_atoi(char first_c, FILE *file, char end)
{
  size_t res = 0;

  char c = first_c;
  res = res * 10 + c - '0';
  while ((c = fgetc(file)) != end)
    res = (res * 10) + c - '0';

  return res;
}

char *parse_string(char first_c, FILE *file, size_t *size)
{
  size_t len = my_atoi(first_c, file, ':');
  char *out = malloc(sizeof(char) * len + 1);
  for (size_t i = 0; i < len; ++i)
   out[i] = fgetc(file);

  out[len] = 0;
  if (size)
    *size = len;
  return out;
}

char *parse_number(FILE *file)
{
  char *res = NULL;
  size_t n = 0;
  ssize_t len = getdelim(&res, &n, 'e', file);
  res[len - 1] = 0;
  return res;
}

struct element *get_elt(char c, char *key, FILE *file)
{
  void *value = NULL;
  enum type t = CHAR;
  size_t size = 0;
  if (c == 'i')
  {
    t = NUMBER;
    value = parse_number(file);
  }
  else if (c == 'd')
  {
    t = DICT;
    value = parse_dict(file);
  }
  else if (c == 'l')
  {
    t = LIST;
    value = parse_list(file);
  }
  else
    value = parse_string(c, file, &size);

  return create_elt(key, value, t, size);
}

struct dictionary *parse_dict(FILE *file)
{
  struct dictionary *dict = create_dict();
  char c = 0;
  while ((c = fgetc(file)) != 'e')
  {
    char *key = parse_string(c, file, NULL);
    c = fgetc(file);
    struct element *elt = get_elt(c, key, file);
    add_elt(dict, elt);
  }
  return dict;
}

struct list *parse_list(FILE *file)
{
  struct list *l = init_list();
  char c = 0;
  while ((c = fgetc(file)) != 'e')
  {
    struct element *elt = get_elt(c, NULL, file);
    add_tail(l, elt);
  }

  return l;
}

struct tracker *parse_file(const char *path)
{
  FILE *file = fopen(path, "r");
  if (!file)
    return NULL;

  struct tracker *tr = malloc(sizeof(struct tracker));

  fgetc(file);
  tr->dict = parse_dict(file);

  fclose(file);
  return tr;
}
