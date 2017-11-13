#include <stdlib.h>

#include "list.h"

/**
** \details   pop the last element of #list
** \param     l   the #list to pop
*/
void *pop_tail(struct list *l)
{
  if (l->head)
  {
    struct node *prev = l->head;
    if (!prev->next)
    {
      l->head = NULL;
      void *tmp = prev->data;
      free(prev);
      --l->size;
      return tmp;
    }

    struct node *cur = prev->next;
    while (cur->next)
    {
      prev = cur;
      cur = cur->next;
    }
    prev->next = NULL;

    void *tmp = cur->data;
    free(cur);

    --l->size;
    return tmp;
  }
  return NULL;
}

/**
** \details   pop the first element of #list
** \param     l   the #list to pop
*/
void *pop_front(struct list *l)
{
  struct node *tmp = l->head;
  if (tmp)
  {
    l->head = tmp->next;
    void *ret = tmp->data;
    free(tmp);
    --l->size;
    return ret;
  }
  return NULL;
}

/**
** \details   pop the element of #list at \a index
** \param     l       the #list to pop
** \param     index   the index to pop
*/
void *pop_at(struct list *l, int index)
{
  if (index == 0)
    return pop_front(l);
  if (index == l->size - 1)
    return pop_tail(l);

  struct node *prev = l->head;
  if (!prev)
    return NULL;
  struct node *cur = prev->next;
  for (int i = 1; i < index; ++i)
  {
    prev = cur;
    cur = cur->next;
  }
  prev->next = cur->next;
  void *res = cur->data;
  free(cur);
  --l->size;
  return res;
}

/**
** \details   add a node to the end of #list
** \param     l     the #list
** \param     node  the #node to add
*/
void add_tail(struct list *l, void *elt) 
{
  struct node *new = create_node(elt);

  struct node *prev = l->head;
  if (!prev)
  {
    ++l->size;
    l->head = new;
    return;
  }
  struct node *cur = prev->next;
  while (cur)
  {
    prev = cur;
    cur = cur->next;
  }
  prev->next = new;
  ++l->size;
}

/**
** \details   add an element to the front of #list
** \param     l     the #list
** \param     node  the #node to add
*/
void add_front(struct list *l, void *elt)
{
  struct node *new = create_node(elt);

  new->next = l->head;
  l->head = new;
  ++l->size;
}
