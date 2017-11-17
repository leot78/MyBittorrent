#include <stdio.h>
#include <string.h>
#include <err.h>

#include "my_bittorrent.h"
#include "dictionary.h"
#include "parsing.h"
#include "print_json.h"

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

void print_hash(unsigned char *hash)
{
  for (int i = 0; i < 20; ++i)
  {
    printf("%02x", hash[i]);
  }
  printf("\n");
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

  //char *url = get_tracker_url(tracker);

  //printf("url: %s\n", url);
  
  unsigned char *hash = get_info_hash(tracker);
  //hash = hash;
  print_hash(hash);

  if (opt & PRINT)
  {
    print_json(tracker);
  }
  if (opt & PEERS)
  {
    printf("--dump-peers option selected: not implemented yet\n");
  }
  if (opt & SEED)
  {
    printf("--seed option selected: not implemented yet\n");
  }
  if (opt & VERBOSE)
  {
    printf("--verbose option selected: not implemented yet\n");
  }

  delete_tracker(tracker);
  return 0;
}
