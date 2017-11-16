#ifndef ENCODE_H
# define ENCODE_H

#include "dictionary.h"

char *bencode_dict(struct dictionary *dict, char *str,
                   size_t *size, size_t *index);

#endif /* !ENCODE_H */
