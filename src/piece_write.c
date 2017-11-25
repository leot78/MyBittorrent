#include <stdio.h>

#include "my_bittorrent.h"

void write_piece(uint32_t index)
{
  struct dictionary *info_dict = get_value(g_client.tracker->dict,
                                           "info", NULL);
  char *name = get_value(info_dict, "name", NULL);
  FILE *f = fopen(name, "w");
  if (!f)
    err(1, "Could not open '%s' for writing", name);
  if ((fseek(f, index * g_client.piece_max_len, SEEK_SET)) == -1)
    err(1, "Could not seek index : %ld", index * g_client.piece_max_len);
  if ((fwrite(g_client.piece, sizeof(char),
             g_client.piece_len, f)) != g_client.piece_len)
    err(1, "Could not write piece");
  if ((fclose(f) == EOF))
    err(1, "Could not close file");
}
