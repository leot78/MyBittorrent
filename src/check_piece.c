#include <stdio.h>
#include <stdlib.h>

#include "my_bittorrent.h"

void *get_hash_piece_from_dict(struct tracker *tr, size_t index)
{
  struct dictionary *info_dict = get_value(tr->dict, "info", NULL);
  char *piece = get_value(info_dict, "pieces", NULL);
  
  for (size_t i = 0; i < index; ++i)
    piece += 20;

  return piece;
}


int check_piece(void *data, size_t len, size_t index)
{
  unsigned char *hash_piece  = compute_sha1(data, len);
  unsigned char *ref_hash_piece = get_hash_piece_from_dict(g_client.tracker,
                                                           index);
  size_t i = 0;
  while (i < 20 && hash_piece[i] == ref_hash_piece[i])
    ++i;

  free(hash_piece);
  return i == 20;
}
