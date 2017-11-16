#include <openssl/evp.h>
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

char *get_tracker_url(struct tracker *tracker)
{
  struct dictionary *dict = tracker->dict;
  struct node *cur = dict->table->head;
  for (; cur; cur = cur->next)
  {
    struct element *elt = cur->data;
    if (strcmp(elt->key, "announce") == 0)
      return elt->value;
  }
  return NULL;
}

void delete_tracker(struct tracker *tr)
{
  delete_dict(tr->dict);
  free(tr->info);
  free(tr);
}

unsigned char *compute_sha1(char *info)
{
  unsigned char *md_value = malloc(EVP_MAX_MD_SIZE * sizeof(char));
  unsigned md_len;
  const EVP_MD *md = EVP_sha1();
  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(mdctx, md, NULL);
  EVP_DigestUpdate(mdctx, info, strlen(info));
  EVP_DigestFinal_ex(mdctx, md_value, &md_len);
  EVP_MD_CTX_free(mdctx);
  return md_value;
}
