#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "my_bittorrent.h"

char *compute_peer_id(void)
{
  static int seeded = 0;
  if (!seeded)
  {
    srand(time(NULL));
    seeded = 1;
  }
  int r = rand();
  char *pre_peer = PEER_ID_PREFIX;
  char *post_peer = itoa(q
