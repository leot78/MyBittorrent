#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_bittorrent.h"

size_t get_end_addr(char *urlp)
{
  size_t i = 0;
  int semic = 0;
  for (; semic != 2; i++)
  {
    if (urlp[i] == ':')
      semic++;
  }
  return i;
}

/*char *get_url(char *urlp, size_t sep_pos)
{
  size_t len_urlp = strlen(urlp);
  char *url = malloc(sizeof(char)
              * (len_urlp + 1 - strlen(urlp + sep_pos)));
  if (!url)
    err(1, "Could not allocate string url");
  url = strncpy(url, urlp, sep_pos);
  url[sep_pos] = '\0';
  return url;
}*/

char *prepare_request(char *port, char *urlp, char *info_hash)
{
  size_t total_size = strlen(urlp) + REQUEST_LEN;
  char *get_request = malloc(sizeof(char) * total_size);
  if (!get_request)
    err(1, "Could not allocate string request");
  sprintf(get_request,
          "%s/announce?peer_id=%s&info_hash=%s\n&port=%s&left=0\
          &downloaded=0&uploaded=0&compact=1",
          urlp, compute_peer_id(), info_hash, port);
  return get_request;
}

char *get_tracker(char *urlp, char *sha1)
{
  size_t sep_pos = get_end_addr(urlp);
  CURL *handle = curl_easy_init();
  char *request = prepare_request(urlp + sep_pos, urlp, sha1);
  char *buff = NULL;
  char errbuff[CURL_ERROR_SIZE];
  curl_easy_setopt(handle, CURLOPT_URL, request);
  //curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buff);
  curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, &errbuff);
  int res = curl_easy_perform(handle);
  curl_easy_cleanup(handle);
  curl_global_cleanup();
  if (res)
    err(res, errbuff);
  return buff;
}



