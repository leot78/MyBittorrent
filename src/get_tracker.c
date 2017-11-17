#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <my_string.h>

#include "my_bittorrent.h"

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  size_t t_size = size * nmemb;
  struct data_chunk *chunk = userdata;

  char *tmp = realloc(chunk->data, chunk->size + t_size + 1);
  if (!tmp)
  {
    warn("Could not reallocate data");
    return 0;
  }
  chunk->data = tmp;
  memcpy(chunk->data + chunk->size, ptr, t_size);
  chunk->size += t_size;
  chunk->data[chunk->size] = '\0';
  return t_size;
}

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

char *prepare_request(char *port, char *urlp, char *info_hash,
                      CURL *handle)
{
  size_t total_size = strlen(urlp) + REQUEST_LEN;
  char *get_request = malloc(sizeof(char) * total_size);
  char *peer_id = compute_peer_id();
  peer_id = curl_easy_escape(handle, peer_id, 0);
  if (!get_request)
    err(1, "Could not allocate string request");
  info_hash = curl_easy_escape(handle, info_hash, 0);
  sprintf(get_request,
          "?peer_id=%s&info_hash=%s&port=6881&left=0",
          peer_id, info_hash);
  get_request = concat(get_request, "&downloaded=0&uploaded=0&compact=1");

  get_request = concat(urlp, get_request);
  return get_request;
}

char *get_tracker(char *urlp, char *sha1)
{
  size_t sep_pos = get_end_addr(urlp);
  CURL *handle = curl_easy_init();
  char *port = malloc(sizeof(char) * 6);
  port = strcpy_delim(port, urlp + sep_pos, '/');
  //urlp = "http://acu-tracker-1.pie.cri.epita.net:6969";
  char *request = prepare_request(port, urlp, sha1, handle);
  struct data_chunk buff;
  buff.size = 0;
  buff.data = malloc(sizeof(char));
  if (!buff.data)
    err(1, "Could not allocate first buff.data for curl GET");

  char errbuff[CURL_ERROR_SIZE];
  curl_easy_setopt(handle, CURLOPT_URL, request);
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buff);
  curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, &errbuff);                                                                                                       
  curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
  int res = curl_easy_perform(handle);
  curl_easy_cleanup(handle);
  curl_global_cleanup();
  
  if (res)
    err(res, errbuff);
  return buff.data;
}
