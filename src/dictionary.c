#include <stdlib.h>
#include <string.h>

#include "dictionary.h"
#include "list/list.h"

struct dictionary *create_dict(void)
{
  struct dictionary *d = malloc(sizeof(struct dictionnary));
  if (!d)
    return NULL;

  d->size = 0;
  d->table = init_list();

  return d;
}

struct element *create_elt(char *key, void *value, enum type t)
{
  struct element *e = malloc(sizeof(struct element));
  e->type = t;
  e->value = value;
  e->key = key;

  return e;
}

void add_dict(struct dictonary *d, char *key, void *value, enum type t)
{
  if (!d)
    return;

  struct element *elt = create_elt(key, value, t);
  add_tail(d->table, elt);
  ++d->size;
}

struct element *get_dict(struct dictionary *d, char *key)
{
  struct node *cur = d->table->head;
  for (; cur; cur = cur->next)
  {
    struct element *elt = cur->data;
    if (strcmp(key, elt->key) == 0)
      return element;
  }

  return NULL;
}

void delete_dict(struct dictionary *d)
{
  while (d->table->size)
  {
    struct element *elt = pop_front(d->table);
    free(elt->key);
    if (elt->type == DICT)
      delete_dict(elt->value);
    else if (elt->type == LIST)
      free_list(elt->value);
    else
      free(elt->value);
  }
  free_list(d->table);
  free(d);
}
