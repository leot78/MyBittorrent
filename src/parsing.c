# define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_bittorrent.h"
#include "parsing.h"
#include "dictionary.h"
#include "list/list.h"

/**
** convert a bencoded integer in number
*/
size_t my_atoi(char first_c, char *str, char end, size_t *index)
{
  size_t res = 0;

  char c = first_c;
  res = res * 10 + c - '0';
  *index += 1;
  while ((c = str[*index]) != end)
  {
    res = (res * 10) + c - '0';
    *index += 1;
  }
  *index += 1;

  return res;
}

/**
** parse a bencoded string
*/
char *parse_string(char first_c, char *str, size_t *size, size_t *index)
{
  size_t len = my_atoi(first_c, str, ':', index);
  char *out = malloc(sizeof(char) * len + 1);
  memcpy(out, str + *index, len);
  *index += len - 1;
  out[len] = 0;
  if (size)
    *size = len;
  return out;
}

/**
** parse a bencoded string
*/
char *parse_number(char *str, size_t *index)
{
  *index += 1;
  size_t i = *index;
  size_t len = 0;
  while (str[i] != 'e')
  {
    ++len;
    ++i;
  }
  char *res = malloc(len + 1);;
  memcpy(res, str + *index, len);
  res[len] = 0;
  *index += len;
  return res;
}

/**
** parse the correct bencoded element
*/
struct element *get_elt(char c, char *key, char *str, size_t *index)
{
  void *value = NULL;
  enum type t = CHAR;
  size_t size = 0;
  if (c == 'i')
  {
    t = NUMBER;
    value = parse_number(str, index);
  }
  else if (c == 'd')
  {
    t = DICT;
    value = parse_dict(str, index);
  }
  else if (c == 'l')
  {
    t = LIST;
    value = parse_list(str, index);
  }
  else
    value = parse_string(c, str, &size, index);

  return create_elt(key, value, t, size);
}

/**
** parse a bencoded dictionary
*/
struct dictionary *parse_dict(char *str, size_t *index)
{
  struct dictionary *dict = create_dict();
  char c = 0;
  *index += 1;
  while ((c = str[*index]) != 'e')
  {
    char *key = parse_string(c, str, NULL, index);
    *index += 1;
    c = str[*index];
    struct element *elt = get_elt(c, key, str, index);
    add_elt(dict, elt);
    *index += 1;
  }
  return dict;
}

/**
** parse a bencoded list
*/
struct list *parse_list(char *str, size_t *index)
{
  struct list *l = init_list();
  char c = 0;
  *index += 1;
  while ((c = str[*index]) != 'e')
  {
    struct element *elt = get_elt(c, NULL, str, index);
    add_tail(l, elt);
    *index += 1;
  }

  return l;
}

/**
** parse the bencoded content in a string
*/
struct tracker *parse_content(char *content)
{
  struct tracker *tr = malloc(sizeof(struct tracker));

  size_t index = 0;
  tr->dict = parse_dict(content, &index);

  return tr;
}

/**
** parse a bencoded file
*/
struct tracker *parse_file(const char *path)
{
  FILE *file = fopen(path, "r");
  if (!file)
    err(1, "cannot open file : %s", path);

  fseek(file, 0, SEEK_END);

  size_t size_file = ftell(file);

  char *content = malloc(size_file + 1);
  if (!content)
    err(1, "cannot malloc in parse_file");

  rewind(file);
  int fr = fread(content, 1, size_file, file);
  if (fr == -1)
    err(1, "cannot fread)");
  content[size_file] = 0;
  fclose(file);

  struct tracker *tr = parse_content(content);
  free(content);
  tr->info_hash = NULL;
  return tr;
}
