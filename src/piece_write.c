#include <stdio.h>

#include "my_bittorrent.h"

/**
** Write piece with given index in file from dictionary.
*/
void write_piece(uint32_t index)
{
  struct dictionary *info_dict = get_value(g_client.tracker->dict,
                                           "info", NULL);
  char *name = get_value(info_dict, "name", NULL);
  FILE *f = fopen(name, "a");
  if (!f)
    err(1, "Could not open '%s' for writing", name);
  size_t offset = index * g_client.piece_max_len;
  /*if (index == g_client.number_piece - 1)
  {
    size_t piece_len = get_len_from_files(info_dict) % g_client.piece_max_len;
    offset = piece_len * g_client.piece_max_len;
  }*/
  if ((fseek(f, offset, SEEK_SET)) == -1)
    err(1, "Could not seek index : %ld", index * g_client.piece_max_len);
  if ((fwrite(g_client.piece, sizeof(char),
             g_client.piece_len, f)) != g_client.piece_len)
    err(1, "Could not write piece");
  if ((fclose(f) == EOF))
    err(1, "Could not close file");
}
