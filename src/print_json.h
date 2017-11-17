#ifndef PRINT_JSON_H
# define PRINT_JSON_H

#include "my_bittorrent.h"
#include "dictionary.h"

void print_json(struct tracker *tr);
void print_json_dict(struct dictionary *d, int pad);
void print_string(const char *s, size_t size);

#endif /* !PRINT_JSON_H */
