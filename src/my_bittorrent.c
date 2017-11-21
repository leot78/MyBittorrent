#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

#include "my_bittorrent.h"
#include "dictionary.h"
#include "list/list.h"
#include "parsing.h"
#include "print_json.h"
#include "print_log.h"
#include "connect_peers.h"

enum options parse_options(int argc, char **argv, int *index)
{
  enum options opt = NONE;
  int i = *index;
  while (i < argc && argv[i][0] == '-')
  {
    if (strcmp("--pretty-print-torrent-file", argv[i]) == 0)
      opt = opt | PRINT;
    else if (strcmp("--dump-peers", argv[i]) == 0)
      opt = opt | PEERS;
    else if (strcmp("--verbose", argv[i]) == 0)
      opt = opt | VERBOSE;
    else if (strcmp("--seed", argv[i]) == 0)
      opt = opt | SEED;
    else
      errx(1, "%s: '%s' unknown option", argv[0], argv[i]);

    ++i;
  }
  *index = i;
  return opt;
}

char *get_hash(unsigned char *hash, size_t size)
{
  char *res = malloc(size * 2 + 1);
  for (size_t i = 0; i < size; ++i)
  {
    sprintf(res + (i * 2), "%02x", hash[i]);
  }
  res[size * 2] = 0;
  return res;
}

int main(int argc, char **argv)
{
  if (argc < 2 || argc > 4)
  {
    printf("Usage : %s [options] [files]\n", argv[0]);
    return 0;
  }
  int index = 1;
  enum options opt = parse_options(argc, argv, &index);
  if (index >= argc)
  {
    printf("Usage : %s [options] [files]\n", argv[0]);
    return 0;
  }

  char *filepath = argv[index];
  struct tracker *tracker = parse_file(filepath);
  //test
    printf("handshake=%s\n", generate_handshake(get_info_hash(tracker)));
  //
  if (opt & PRINT)
  {
    print_json(tracker);
    delete_tracker(tracker);
    return 0;
  }
  
  init_log(opt & VERBOSE);
  struct list *peer_list = get_peers(tracker);

    
  if (opt & PEERS)
  {
    if (peer_list->size == 0)
      return 1;
    print_peers(peer_list);
    return 0;
  }
  
  int arr_sock[50];
  int epoll_fd =   create_epoll(peer_list, arr_sock);
  handle_epoll_event(epoll_fd, arr_sock, peer_list);

  if (opt & SEED)
  {
    printf("--seed option selected: not implemented yet\n");
  }
  delete_tracker(tracker);
  free_sock_list(peer_list);
  delete_log_info();
  return 0;
}
