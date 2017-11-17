#ifndef ENCODE_H
# define ENCODE_H

#include "dictionary.h"

char *bencode_dict(struct dictionary *dict, char *str, size_t *index);
char *get_bencode(struct dictionary *dict, size_t *len);

#endif /* !ENCODE_H */
