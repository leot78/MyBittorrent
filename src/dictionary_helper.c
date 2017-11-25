#include <err.h>
#include <stdlib.h>

#include "math.h"
#include "dictionary.h"
#include "list/list.h"
#include "my_string.h"

size_t get_len_from_files(struct dictionary *dict)
{
  char *str_len = get_value(dict, "length", NULL);
  if (str_len)
    return atoi(str_len);
  struct list *file_list = get_value(dict, "files", NULL);
  if (!file_list)
    err(1, "cannot find files list");

  size_t len = 0;

  for (struct node *cur = file_list->head; cur; cur = cur->next)
  {
    struct element *tmp = cur->data;
    struct dictionary *tmp_dict = tmp->value;
    len += atoi(get_value(tmp_dict, "length", NULL));
  }

  return len;
}

size_t get_nb_piece(struct dictionary *dict)
{
  size_t len = get_len_from_files(dict);

  size_t piece_len = atoi(get_value(dict, "piece length", NULL));

  if (len % piece_len == 0)
    return len / piece_len;
  return (len / piece_len) + 1;
}
