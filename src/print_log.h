#ifndef PRINT_LOG_H
# define PRINT_LOG_H

struct log_info
{
  char *torrent_id;
  int log_active;
};

void print_log(char *action, char *msg);
void set_torrent_id(char *hash_str);
void init_log(int active);
void delete_log_info(void);
#endif /* !PRINT_LOG_H */
