#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "my_bittorrent.h"
#include "my_string.h"

char *compute_peer_id(void)
{
  static int seeded = 0;
  if (!seeded)
  {
    srand(time(NULL));
    seeded = 1;
  }
  int r = rand();
  double min_size = pow(10, PEER_ID_POSTLEN - 1);
  double max_size = pow(10, PEER_ID_POSTLEN) - 1;
  long diff = max_size - min_size;
  double factor = diff / RAND_MAX;
  long twelve_dig = r * factor + min_size;
  printf("rand = %ld\n", twelve_dig);

  char *pre_peer = PEER_ID_PREFIX;
  char *post_peer = my_itoa(twelve_dig);
  char *peer = concat(pre_peer, post_peer);
  free(post_peer);
  return peer;
}
