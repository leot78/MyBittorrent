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

void *pop_elt(struct list *l, void *elt)
{
  struct node *prev = l->head;

  if (prev->data == elt)
    pop_front(l);
    

  for (struct node *cur = l->head; cur; cur = cur->next)
  {
    if (cur->data = elt)
    {
      prev->next = cur->next;
      void *res = cur->data;
      free(cur);
      --l->size;
      return res;
    }
    prev = cur;
  }
  return NULL;
}
