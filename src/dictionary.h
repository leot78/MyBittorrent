#ifndef DICTIONARY_H
# define DICTIONARY_H

enum type
{
  DICT,
  LIST,
  CHAR,
  NUMBER
};

struct element
{
  enum type type;
  void *value;
  size_t size;
  char *key;
};

struct dictionary
{
  size_t size;
  struct list *table;
};

struct dictionary *create_dict(void);
void add_elt(struct dictionary *d, struct element *elt);
void *get_value(struct dictionary *d, char *key, size_t *size);
void delete_dict(struct dictionary *d);
struct element *create_elt(char *key, void *value, enum type t, size_t size);

#endif /* !DICTIONARY_H */
