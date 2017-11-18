#ifndef PARSING_H
# define PARSING_H

struct dictionary *parse_dict(char *str, size_t *index);
struct list *parse_list(char *str, size_t *index);
struct tracker *parse_file(const char *path);
struct tracker *parse_content(char *content);

#endif /* PARSING_H */
