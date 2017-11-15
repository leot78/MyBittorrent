#ifndef MY_BITTORRENT_H
# define MY_BITTORRENT_H

#define PEER_ID_PREFIX "-MB2020-"
#define PEER_ID_POSTLEN 12
#define PEER_ID_MINVALUE 100000000000
#define PEER_ID_MAXVALUE 999999999999

enum options
{
  NONE = 0x000,
  PRINT = 0x001,
  VERBOSE = 0x010,
  PEERS = 0x002,
  SEED = 0x100
};

struct tracker
{
  char *url;
  struct dictionnary *info;
};

char *compute_peer_id(void);

#endif /*! MY_BITTORRENT_H */
