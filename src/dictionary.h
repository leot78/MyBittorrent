#ifndef DICTIONARY_H
# define DICTIONARY_H

enum type
{
  DICT,
  LIST,
  CHAR
};

struct element
{
  enum type type;
  void *value;
  char *key;
};

struct dictionary
{
  size_t size;
  struct list *table;
};

struct dictionary *create_dict(void);
void add_dict(struct dictonary *d, char *key, void *value, enum type t);
struct element *get_dict(struct dictionary *d, char *key);
void delete_dict(struct dictionary *d);

#endif /* !DICTIONARY_H */
