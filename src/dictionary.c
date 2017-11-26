#include <stdlib.h>
#include <string.h>

#include "dictionary.h"
#include "list/list.h"

/**
** create a dictionary
*/
struct dictionary *create_dict(void)
{
  struct dictionary *d = malloc(sizeof(struct dictionary));
  if (!d)
    return NULL;

  d->size = 0;
  d->table = init_list();

  return d;
}

/**
** create element
*/
struct element *create_elt(char *key, void *value, enum type t, size_t size)
{
  struct element *e = malloc(sizeof(struct element));
  e->type = t;
  e->value = value;
  e->key = key;
  e->size = size;

  return e;
}

/**
** add an element in the dictionary
*/
void add_elt(struct dictionary *d, struct element *elt)
{
  if (!d)
    return;

  add_tail(d->table, elt);
  ++d->size;
}

/**
** get the value from the key in a dictionary
*/
void *get_value(struct dictionary *d, char *key, size_t *size)
{
  struct node *cur = d->table->head;
  for (; cur; cur = cur->next)
  {
    struct element *elt = cur->data;
    if (strcmp(key, elt->key) == 0)
    {
      if (size)
        *size = elt->size;
      return elt->value;
    }
  }
  return NULL;
}

/**
** delete all element in a list
*/
void delete_list(struct list *l)
{
  while (l->size)
  {
    struct element *elt = pop_front(l);
    if (elt->type == DICT)
      delete_dict(elt->value);
    else if (elt->type == LIST)
      delete_list(elt->value);
    else
      free(elt->value);
    free(elt);
  }
  free(l);
}

/**
** delete all element in a dictionary
*/
void delete_dict(struct dictionary *d)
{
  while (d->table->size)
  {
    struct element *elt = pop_front(d->table);
    free(elt->key);
    if (elt->type == DICT)
      delete_dict(elt->value);
    else if (elt->type == LIST)
      delete_list(elt->value);
    else
      free(elt->value);
    free(elt);
  }
  free_list(d->table);
  free(d);
}
