#ifndef MY_BITTORRENT_H
# define MY_BITTORRENT_H

struct tracker
{
  char *url;
  struct dictionnary *info;
};

#endif /*! MY_BITTORRENT_H */
