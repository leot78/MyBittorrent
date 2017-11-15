#include <stdio.h>
#include <string.h>
#include <err.h>

#include "my_bittorrent.h"
#include "dicionary.h"
#include "parsing.h"


enum options parse_options(int argc, char **argvi, int *index)
{
  enum options opt = NONE;
  int i = *index;
  while (argv[i][0] == '-')
  {
    if (strcmp("--pretty-print-torrent", argv[i]) == 0)
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

int main(int argc, char **argv)
{
  if (argc < 2 || argc > 4)
  {
    printf("Usage : %s [options] [files]\n", argv[0]);
    return 0;
  }
  int index = 1;
  enum options opt = parse_options(argc, argv, &index);
  char *filepath = argv[index];

  struct dictionnary *dict = parse_file(filepath);

  if (opt & PRINT)
  {
    print_json_dict(dict, 0);
  }
  else if (opt & PEERS)
  {
    //DUMP-PEERS
  }
  else if (opt & SEED)
  {
    //SEED
  }

  delete_dict(dict);
  return 0;
}
