#include <stdio.h>

#include "dicionary.h"
#include "parsing.h"




int main(int argc, char **argv)
{
  if (argc < 2 || argc > 4)
  {
    printf("Usage : %s [options] [files]\n", argv[0]);
    return 0;
  }
  else
  {
    char *filepath;
    char *option;
    if (argc == 2)
      filepath = argv[1];
    else if (argc == 3)
      filepath = argv[2];

    struct dictionary *dict = parse_file(filepath);

