#ifndef MY_BITTORRENT_H
# define MY_BITTORRENT_H

#define PEER_ID_PREFIX "-MB2020-"
#define PEER_ID_POSTLEN 12
#define PEER_ID_LEN 20
#define PEER_ID_MINVALUE 100000000000
#define PEER_ID_MAXVALUE 999999999999

#define REQUEST_LEN 124

#include <err.h>
#include <stddef.h>

#include "dictionary.h"

enum options
{
  NONE = 0x000,
  PRINT = 0x001,
  VERBOSE = 0x010,
  PEERS = 0x002,
  SEED = 0x100
};

struct data_chunk
{
  char *data;
  size_t size;
};

struct tracker
{
  struct dictionary *dict;
  char *info;
};

/*struct raw_addr
{
  uint32 ip;
  uint16 
*/
char *compute_peer_id(void);
char *get_tracker_url(struct tracker *tracker);
void delete_tracker(struct tracker *tr);
unsigned char *compute_sha1(char *info, size_t len);
unsigned char *get_info_hash(struct tracker *tr);
char *get_tracker(char *urlp, unsigned char *sha1);
void dump_peers(struct tracker *tracker);
void print_hash(unsigned char *hash);

#endif /*! MY_BITTORRENT_H */
