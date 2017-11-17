#include <stdio.h>

#include "my_bittorrent.h"

void dump_peers(struct tracker *tracker)
{
  char *hash = get_info_hash(tracker);
  char *urlp = get_tracker_url(tracker);
  char *tracker_response = get_tracker(urlp, hash);
  printf("raw tracker response :\n%s", tracker_response);
}
