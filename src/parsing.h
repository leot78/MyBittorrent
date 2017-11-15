#ifndef PARSING_H
# define PARSING_H


struct dictionary *parse_dict(FILE *file);
struct list *parse_list(FILE *file);
struct dictionary *parse_file(const char *path);
void print_json_dict(struct dictionary *d, int pad);
void print_json_list(struct list *l, int pad);

#endif /* PARSING_H */
