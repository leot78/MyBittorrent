#include <stdlib.h>

#include "list.h"

/**
** \details   create a new #node that contain \a data
** \param     data  the date to store in the new #node
** \return    returns the new #node or NULL on error
*/
struct node *create_node(void *data)
{
  struct node *node = malloc(sizeof(struct node));
  node->data = data;
  node->next = NULL;
  return node;
}

/**
** \details   initialize a new #list
** \return    returns the new #list or NULL on error
*/
struct list *init_list(void)
{
  struct list *l = malloc(sizeof(struct list));
  if (!l)
    return NULL;
  l->head = NULL;
  l->size = 0;
  return l;
}

/**
** \details   empties a #list
** \param     l   the #list to clear
*/
void clear_list(struct list *l)
{
  while (l->size)
    pop_front(l);
}

/**
** \details   free a #list
** \param     l   the #list to free
*/
void free_list(struct list *l)
{
  if (l)
  {
    clear_list(l);
    free(l);
    l = NULL;
  }
}
