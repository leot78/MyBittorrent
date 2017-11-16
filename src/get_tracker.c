#include <curl/curl.h>
#include <string.h>

#include "my-bittorrent.h"

size_t get_end_addr(char *urlp)
{
  size_t i = 0;
  int semic = 0;
  for (; semic != 2; i++)
  {
    if (urlp[i] == ':')
      semic++;
  }
  return i - 1;
}

char *get_url(char *urlp, size_t sep_pos)
{
  size_t len_urlp = strlen(urlp);
  char *url = malloc(sizeof(char)
              * (len_urlp - strlen(urlp + sep_pos)));
  if (!url)
    err(1, "Could not allocate string url");
  url = strncpy(url, urlp, sep_pos);
  return url;

char *get_response(char *urlp)
{
  size_t sep_pos = get_end_add(urlp);
  char *url = get_url(urlp, sep_pos);
  CURL *handle = curl_easy_init();
  char *buff = NULL;
  char errbuff[CURL_ERROR_SIZE];
  curl_easy_setopt(handle, CURLOPT_URL, url);
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(handle, CURL_
