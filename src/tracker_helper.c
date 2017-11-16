#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "dictionary.h"
#include "list/list.h"
#include "my_bittorrent.h"
#include "my_string.h"
#include "string.h"

char *compute_peer_id(void)
{
  static int seeded = 0;
  if (!seeded)
  {
    srand(time(NULL));
    seeded = 1;
  }
  int r = rand();
  long diff = PEER_ID_MAXVALUE - PEER_ID_MINVALUE;
  double factor = diff / RAND_MAX;
  long twelve_dig = r * factor + PEER_ID_MINVALUE;
  printf("rand = %ld\n", twelve_dig);

  char *pre_peer = PEER_ID_PREFIX;
  char *post_peer = my_itoa(twelve_dig);
  char *peer = concat(pre_peer, post_peer);
  free(post_peer);
  return peer;
}

char *get_tracker_url(struct dictionary *dict)
{
  struct node *cur = dict->table->head;
  for (; cur; cur = cur->next)
  {
    struct element *elt = cur->data;
    if (strcmp(elt->key, "announce") == 0)
      return elt->value;
  }
  return NULL;
}
