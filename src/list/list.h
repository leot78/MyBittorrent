#ifndef LIST_H
# define LIST_H

struct node
{
  void *data;
  struct node *next;
};

struct list
{
  int size;
  struct node *head;
};

struct node *create_node(void *data);
void add_tail(struct list *l, void *elt);
void add_front(struct list *l, void *elt);
struct list *init_list(void);
void free_list(struct list *l);
void *pop_tail(struct list *l);
void *pop_front(struct list *l);
void *pop_at(struct list *l, int index);

#endif /* LIST_H */
