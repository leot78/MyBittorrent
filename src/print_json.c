#include <stdio.h>
#include <string.h>

#include "dictionary.h"
#include "list/list.h"
#include "my_bittorrent.h"
#include "print_json.h"

void print_string(const char *s, size_t size)
{
  if (size > 100)
    printf("<too much data>");
  else if (!size)
    printf("%s", s);
  else
  {
    size_t i = 0;
    while (i < size)
    {
      if (s[i] < 0x20 || s[i] > 0x7E)
        printf("\\u%04X", s[i]);
      else if (s[i] == '\"')
        printf("\\\"");
      else
        putchar(s[i]);
      ++i;
    }
  }
}

void print_json_list(struct list *l, int pad)
{
  //printf("%*s[", pad, "");
  printf("[ ");
  struct node *cur = l->head;
  for (; cur; cur = cur->next)
  {
    struct element *elt = cur->data;

    if (elt->type == CHAR || elt->type == NUMBER)
    {
      char *quote = (elt->type == NUMBER) ? "" : "\"";
      printf("%s", quote);
      print_string(elt->value, elt->size);
      printf("%s", quote);
    }
    else if (elt->type == DICT)
    {
      printf("\n");
      struct dictionary *dict = elt->value;
      print_json_dict(dict, pad + 4);
    }
    else if (elt->type == LIST)
    {
      struct list *l = elt->value;
      print_json_list(l, pad + 8);
    }
    if (cur->next)
      printf(",");
    printf(" ");
  }
  printf("] ");
}


void print_json_dict(struct dictionary *d, int pad)
{
  printf("%*s{\n", pad, "");
  struct node *cur = d->table->head;
  for (; cur; cur = cur->next)
  {
    struct element *elt = cur->data;
    printf("%*s\"", pad + 4, "");
    print_string(elt->key, 0);
    printf("\" : ");

    if (elt->type == CHAR || elt->type == NUMBER)
    {
      char *quote = (elt->type == NUMBER) ? "" : "\"";
      printf("%s", quote);
      print_string(elt->value, elt->size);
      printf("%s", quote);
    }
    else if (elt->type == DICT)
    {
      printf("\n");
      struct dictionary *dict = elt->value;
      print_json_dict(dict, pad + 8 + strlen(elt->key));
    }
    else if (elt->type == LIST)
    {
      //printf("\n");
      struct list *l = elt->value;
      print_json_list(l, pad + 8 + strlen(elt->key));
    }
    if (cur->next)
      printf(",");
    printf("\n");
  }
  printf("%*s}", pad, "");
}

void print_json(struct tracker *tr)
{
  print_json_dict(tr->dict, 0);
  printf("\n");
}
