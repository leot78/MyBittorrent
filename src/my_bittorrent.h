#ifndef MY_BITTORRENT_H
# define MY_BITTORRENT_H

#define PEER_ID_PREFIX "-MB2020-"
#define PEER_ID_POSTLEN 12
#define PEER_ID_LEN 20
#define PEER_ID_MINVALUE 100000000000
#define PEER_ID_MAXVALUE 999999999999

#define HOST_LEN 1024
#define SERVICE_LEN 20

#define REQUEST_LEN 124

#include <err.h>
#include <stddef.h>
#include <stdint.h>

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

struct raw_addr
{
  uint32_t ip;
  uint16_t port;
} __attribute__ ((packed));

struct peer
{
  struct sockaddr_in *sa;
  int *have;
  int nb_pieces;
  int interested;
  int choked;
  int index_socket;
  char *url;
};

char *compute_peer_id(void);
char *get_tracker_url(struct tracker *tracker);
void delete_tracker(struct tracker *tr);
unsigned char *compute_sha1(char *info, size_t len);
unsigned char *get_info_hash(struct tracker *tr);

char *get_tracker(char *urlp, unsigned char *sha1);

void get_peers_url(struct sockaddr_in *sa, char *host, char *port);
void set_peers_url(struct peer *peer);
void print_peers(struct list *peer_list);
struct list *get_peers(struct tracker *tracker);

void dump_peers(struct tracker *tracker);
char *get_hash(unsigned char *hash, size_t size);

struct peer *create_sock(struct raw_addr *ra, int nb_pieces);
struct list *decode_bin(char *binaries, int nb_pieces);
void free_sock_list(struct list *l_sa);

#endif /*! MY_BITTORRENT_H */
