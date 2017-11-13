#ifndef MY_BITTORRENT_H
# define MY_BITTORRENT_H

#define PEER_ID_PREFIX "-MB2020-"
#define PEER_ID_POSTLEN 12

struct tracker
{
  char *url;
  struct dictionnary *info;
};

#endif /*! MY_BITTORRENT_H */
