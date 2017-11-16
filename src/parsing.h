#ifndef PARSING_H
# define PARSING_H

struct dictionary *parse_dict(FILE *file);
struct list *parse_list(FILE *file);
struct tracker *parse_file(const char *path);

#endif /* PARSING_H */
