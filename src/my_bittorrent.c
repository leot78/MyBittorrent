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

void init_client(struct tracker *tracker)
{
  g_client.tracker = tracker;
  struct dictionary *info_dict = get_value(tracker->dict, "info", NULL);
  g_client.piece_max_len = atoi(get_value(info_dict, "piece length", NULL));
  g_client.number_piece = get_nb_piece(info_dict);
  g_client.have = calloc(g_client.number_piece, sizeof(int));
  if (!g_client.have)
    err(1, "Cound not allocate have list of client struct");
  g_client.piece_len = 0;
  g_client.requested = 0;
  g_client.request_id = -1;
  g_client.finish = 0;
  g_client.piece = calloc(g_client.piece_max_len, sizeof(char));
  if (!g_client.piece)
    err(1, "Could not allocate piece buffer");
  g_client.peer_id = compute_peer_id();
}

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

void delete_client(void)
{
  free(g_client.have);
  free(g_client.piece);
  free(g_client.peer_id);
}

int opt_peers(struct list *peer_list, struct tracker *tracker)
{
  if (peer_list->size == 0)
    return 1;
  print_peers(peer_list);
  free_sock_list(peer_list);
  delete_tracker(tracker);
  delete_client();
  return 0;
}

int opt_print(struct tracker *tracker)
{
  print_json(tracker);
  delete_tracker(tracker);
  delete_client();
  return 0;
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
  init_client(tracker);
  
  if (opt & PRINT)
    return opt_print(tracker);

  init_log(opt & VERBOSE);
  struct list *peer_list = get_peers(tracker);


  if (opt & PEERS)
    return opt_peers(peer_list, tracker);

  int epoll_fd = create_epoll(peer_list);
  handle_epoll_event(epoll_fd, peer_list);

  if (opt & SEED)
    printf("--seed option selected: not implemented yet\n");
  
  delete_tracker(tracker);
  free_sock_list(peer_list);
  delete_log_info();
  delete_client();
  return 0;
}
