#include <stdlib.h>

#include "list.h"

void *get_elt_at(struct list *l, size_t index)
{
  size_t i = 0;
  for (struct node *cur = l->head; cur; cur = cur->next)
  {
    if (i == index)
      return cur->data;
    if (i > index)
      return NULL;
    i++;
  }
  return NULL;
}
