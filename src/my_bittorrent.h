#ifndef MY_BITTORRENT_H
# define MY_BITTORRENT_H

struct tracker
{
  char *ip;
  char *por;
  char *left;
  char *info_hash;
