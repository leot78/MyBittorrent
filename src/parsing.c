# define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parsing.h"
#include "dictionary.h"
#include "list/list.h"

size_t my_atoi(char first_c, FILE *file, char end)
{
  size_t res = 0;

  char c = first_c;
  res = res * 10 + c - '0';
  while ((c = fgetc(file)) != end)
    res = (res * 10) + c - '0';

  return res;
}

char *parse_string(char first_c, FILE *file)
{
  size_t len = my_atoi(first_c, file, ':');
  char *out = malloc(sizeof(char) * len + 1);
  for (size_t i = 0; i < len; ++i)
   out[i] = fgetc(file);

  out[len] = 0;
  return out;
}

char *parse_number(FILE *file)
{
  char *res = NULL;
  size_t n = 0;
  ssize_t len = getdelim(&res, &n, 'e', file);
  res[len - 1] = 0;
  return res;
}

struct dictionary *parse_dict(FILE *file)
{
  struct dictionary *dict = create_dict();
  char c = 0;
  while ((c = fgetc(file)) != 'e')
  {
    char *key = parse_string(c, file);
    void *value = NULL;

    enum type t = CHAR;
    c = fgetc(file);
    if (c == 'i')
      value = parse_number(file);
    else if (c == 'd')
    {
      t = DICT;
      value = parse_dict(file);
    }
    else if (c == 'l')
    {
      t = LIST;
      value = parse_list(file);
    }
    else
      value = parse_string(c, file); 

    add_elt(dict, key, value, t);
  }
  return dict;
}

struct list *parse_list(FILE *file)
{
  struct list *l = init_list();
  char c = 0;
  while ((c = fgetc(file)) != 'e')
  {
    void *value = NULL;
    enum type t = CHAR;
    if (c == 'i')
      value = parse_number(file);
    else if (c == 'd')
    {
      t = DICT;
      value = parse_dict(file);
    }
    else if (c == 'l')
    {
      t = LIST;
      value = parse_list(file);
    }
    else
      value = parse_string(c, file);

    add_tail(l, create_elt(NULL, value, t));
  }

  return l;
}

void print_string(const char *s)
{
  size_t i = 0;
  while (s[i] != '\0')
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

void print_json_list(struct list *l, int pad)
{
  //printf("%*s[", pad, "");
  printf("[ ");
  struct node *cur = l->head;
  for (; cur; cur = cur->next)
  {
    struct element *elt = cur->data;

    if (elt->type == CHAR)
    {
      char *value = elt->value;
      //printf("%*s\"", pad + 4, "");
      print_string(value);
      printf("\"");
    }
    else if (elt->type == DICT)
    {
      printf("\n");
      struct dictionary *dict = elt->value;
      print_json_dict(dict, pad + );
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
  //printf("%*s]", pad, "");
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
    print_string(elt->key);
    printf("\" : ");

    if (elt->type == CHAR)
    {
      char *value = elt->value;
      printf("\"");
      print_string(value);
      printf("\"");
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


int main(int argc, char **argv)
{
  if (argc < 2)
    return 0;

  FILE *file = fopen(argv[1], "r");

  fgetc(file);
  struct dictionary *dict = parse_dict(file);

  print_json_dict(dict, 0);
  delete_dict(dict);
}
