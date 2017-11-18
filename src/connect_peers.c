#include <err.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "my_bittorrent.h"
#include "list/list.h"
#include "print_log.h"
#include "my_string.h"

int init_epoll(void)
{
  int epoll_fd = epoll_create(1);
  if (epoll_fd == -1)
    err(1, "cannot create epoll");
  return epoll_fd;
}

void print_peers_connect_log(struct peer *p)
{
  char host[HOST_LEN];
  char service[SERVICE_LEN];
  get_peers_url(p->sa, host, service);
  char *tmp = concat(host, ":");
  char *url = concat(tmp, service);
  free(tmp);
  char *msg = concat("connect: ", url);
  free(url);
  print_log("peers", msg);
  free(msg);
}


int create_epoll(struct list *l_peers, int *arr_sock)
{
  int epoll_fd = init_epoll();
  size_t index = 0;

  for (struct node *cur = l_peers->head; cur; cur = cur->next)
  {
    struct peer *peer = cur->data;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
      err(1, "cannot create socket");

    fcntl(sock, F_SETFL, O_NONBLOCK);

    int co = connect(sock, (struct sockaddr *) peer->sa,
                     sizeof(struct sockaddr));

    if (co != -1 || co & EINPROGRESS)
    {
      struct epoll_event ev;
      ev.events = EPOLLIN | EPOLLOUT;
      ev.data.fd = sock;
      epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev);
      arr_sock[index] = sock;
      peer->index_socket = index;
      index++;
      print_peers_connect_log(peer);
    }
  }
  return epoll_fd;
}
