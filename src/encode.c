#include <stdlib.h>
#include <string.h>

#include "my_string.h"
#include "encode.h"
#include "dictionary.h"
#include "list/list.h"
#include "my_bittorrent.h"

static size_t size;

/**
** get the number of digits in a number
*/
size_t nb_len(size_t nb)
{
  size_t len = 1;
  while (nb > 9)
  {
    nb /= 10;
    ++len;
  }

  return len;
}

/**
** bencode a number
*/
char *bencode_number(char *value, char *str, size_t *index)
{
  size_t number = atoi(value);
  size_t size_nb = nb_len(number);
  size += size_nb + 2;
  str = realloc(str, size);

  str[*index] = 'i';
  *index += 1;

  strncpy(str + *index, value, size_nb);
  *index += size_nb;

  str[*index] = 'e';
  *index += 1;
  return str;
}

/**
** bencode a string
*/
char *bencode_string(char *value, size_t len, char *str, size_t *index)
{
  size_t size_nb = nb_len(len);

  size += size_nb + 1 + len;
  str = realloc(str, size);

  char *size_str = my_itoa(len);
  strncpy(str + *index, size_str, size_nb);
  *index += size_nb + 1;
  free(size_str);
  str[*index - 1] = ':';

  if (len)
    memcpy(str + *index, value, len);

  *index += len;
  return str;
}

/**
** bencode a list
*/
char *bencode_list(struct list *l, char *str, size_t *index)
{
  size += 2;
  str = realloc(str, size);

  str[*index] = 'l';
  *index += 1;

  struct node *cur = l->head;
  for (; cur; cur = cur->next)
  {
    struct element *elt = cur->data;
    if (elt->type == CHAR)
      str = bencode_string(elt->value, elt->size, str, index);
    else if (elt->type == LIST)
      str = bencode_list(elt->value, str, index);
    else if (elt->type == DICT)
      str = bencode_dict(elt->value, str, index);
    else if (elt->type == NUMBER)
      str = bencode_number(elt->value, str, index);
  }

  str[*index] = 'e';
  *index += 1;

  return str;
}

/**
** bencode a dictionary
*/
char *bencode_dict(struct dictionary *dict, char *str, size_t *index)
{
  size += 2;
  str = realloc(str, size);

  str[*index] = 'd';
  *index += 1;

  struct node *cur = dict->table->head;
  for (; cur; cur = cur->next)
  {
    struct element *elt = cur->data;
    str = bencode_string(elt->key, strlen(elt->key), str, index);

    if (elt->type == CHAR)
      str = bencode_string(elt->value, elt->size, str, index);
    else if (elt->type == LIST)
      str = bencode_list(elt->value, str, index);
    else if (elt->type == DICT)
      str = bencode_dict(elt->value, str, index);
    else if (elt->type == NUMBER)
      str = bencode_number(elt->value, str, index);
  }

  str[*index] = 'e';
  *index += 1;

  return str;
}

/**
** get the bencode string of a dictionary
*/
char *get_bencode(struct dictionary *dict, size_t *len)
{
  size_t index = 0;
  char *str = bencode_dict(dict, NULL, &index);
  *len = size;
  return str;
}
