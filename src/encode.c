#include <stdlib.h>
#include <string.h>

#include "my_string.h"
#include "encode.h"
#include "dictionary.h"
#include "list/list.h"
#include "my_bittorrent.h"

size_t nb_len(size_t nb)
{
  size_t len = 1;
  while (nb < 9)
  {
    nb /= 10;
    ++len;
  }

  return len;
}

char *bencode_string(char *value, size_t len, char *str, size_t *size)
{
  size_t size_nb = nb_len(len);
  str = realloc(str, *size + size_nb + 1 + len);
  char *size_str = my_itoa(len);
  strncpy(str + *size, size_str, size_nb);
  *size += size_nb + 1;
  free(size_str);
  str[*size - 1] = ':';

  if (len)
    strncpy(str + *size, value, len);
  
  *size += len;
  return str;
}

char *bencode_list(struct list *l, char *str,
                   size_t *size, size_t *index)
{
  str = realloc(str, *size + 2);

  *size += 1;
  *index += 1;
  str[*index] = 'l';

  struct node *cur = l->head;
  for (; cur; cur = cur->next)
  {
    struct element *elt = cur->data;
    str = bencode_string(elt->key, strlen(elt->key), str, size);
    *index = *size - 1;

    if (elt->type == CHAR)
    {
      str = bencode_string(elt->value, elt->size, str, size);
      *index = *size - 1;
    }
    else if (elt->type == LIST)
      str = bencode_list(elt->value, str, size, index);
    else if (elt->type == DICT)
      str = bencode_dict(elt->value, str, size, index);
  }

  *index += 1;
  str[*index] = 'e';

  *size += 1;
  return str;
}

char *bencode_dict(struct dictionary *dict, char *str,
                   size_t *size, size_t *index)
{
  str = realloc(str, *size + 2);

  *size += 1;
  *index += 1;
  str[*index] = 'd';

  struct node *cur = dict->table->head;
  for (; cur; cur = cur->next)
  {
    struct element *elt = cur->data;
    str = bencode_string(elt->key, strlen(elt->key), str, size);
    *index = *size - 1;

    if (elt->type == CHAR)
    {
      str = bencode_string(elt->value, elt->size, str, size);
      *index = *size - 1;
    }
    else if (elt->type == LIST)
      str = bencode_list(elt->value, str, size, index);
    else if (elt->type == DICT)
      str = bencode_dict(elt->value, str, size, index);
  }

  *index += 1;
  str[*index] = 'e';

  *size += 1;
  return str; 
}
