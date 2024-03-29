#include <openssl/evp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "print_json.h"
#include "dictionary.h"
#include "encode.h"
#include "list/list.h"
#include "my_bittorrent.h"
#include "my_string.h"
#include "string.h"

/**
** Generate a peer_id, with random ending numbers.
*/
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

  char *pre_peer = PEER_ID_PREFIX;
  char *post_peer = my_itoa(twelve_dig);
  char *peer = concat(pre_peer, post_peer);
  free(post_peer);
  return peer;
}

/**
** Return url of from tracker.
*/
char *get_tracker_url(struct tracker *tracker)
{
  return get_value(tracker->dict, "announce", NULL);
}

void delete_tracker(struct tracker *tr)
{
  delete_dict(tr->dict);
  if (tr->info_hash)
    free(tr->info_hash);
  free(tr);
}

/**
** Compute SHA1 from given string.
*/
unsigned char *compute_sha1(char *info, size_t len)
{
  unsigned char *md_value = malloc(EVP_MAX_MD_SIZE * sizeof(char));
  unsigned md_len;
  const EVP_MD *md = EVP_sha1();
  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(mdctx, md, NULL);
  EVP_DigestUpdate(mdctx, info, len);
  EVP_DigestFinal_ex(mdctx, md_value, &md_len);
  EVP_MD_CTX_free(mdctx);
  return md_value;
}

/**
** Return the info_dictionary from dictionary.
*/
struct dictionary *get_info_dict(struct dictionary *d)
{
  return get_value(d, "info", NULL);
}

/**
** Get info_dictionary from tracker and return its SHA1.
*/
unsigned char *get_info_hash(struct tracker *tr)
{
  struct dictionary *info_dict = get_info_dict(tr->dict);

  size_t size = 0;
  char *info_encode = get_bencode(info_dict, &size);
  unsigned char *hash = compute_sha1(info_encode, size);
  tr->info_hash = hash;
  free(info_encode);
  return hash;
}
