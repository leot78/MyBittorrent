#include <err.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

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

void print_peers_connect_log(struct peer *p, char *action)
{
  if (!log_is_active())
    return;

  char *msg = concat(action, p->url);
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

    if (co != -1 || errno == EINPROGRESS)
    {
      struct epoll_event ev;
      ev.events = EPOLLIN | EPOLLOUT;
      ev.data.fd = sock;
      epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev);
      arr_sock[index] = sock;
      peer->index_socket = index;
      print_peers_connect_log(peer, "connect :");
    }
    index++;
  }
  return epoll_fd;
}

int get_socket_index(int *arr_sock, int sock)
{
  size_t i = 0;
  for (; i < 50; i++)
  {
    if (arr_sock[i] == sock)
      return i;
  }
  return -1;
}

void handle_epoll_event(int epoll_fd, int *arr_sock, struct list *l_peer)
{
  struct epoll_event *events = malloc(sizeof(struct epoll_event) * 50);
  if (!events)
    err(1, "cannot malloc struct epoll_event");

  int ndfs = 0;
  while ((ndfs = epoll_wait(epoll_fd, events, 50, 1000)) != 0)
  {
    printf("ndfs : %d\n", ndfs);
    for (int i = 0; i < ndfs; i++)
    {
      if (events[i].events & EPOLLHUP)
      {
        int sock_index = get_socket_index(arr_sock, events[i].data.fd);
        struct peer *p = get_elt_at(l_peer, sock_index);
        print_peers_connect_log(p, "disconnect: ");
      }
      else if (events[i].events & EPOLLIN)
      {
        int sock_index = get_socket_index(arr_sock, events[i].data.fd);
        struct peer *p = get_elt_at(l_peer, sock_index);
        print_peers_connect_log(p, "recv: ");
      }
      else if (events[i].events & EPOLLOUT)
      {
        int sock_index = get_socket_index(arr_sock, events[i].data.fd);
        struct peer *p = get_elt_at(l_peer, sock_index);
        print_peers_connect_log(p, "send: ");
      }
    }
  }
}
