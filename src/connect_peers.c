#include <arpa/inet.h>
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
#include "print_msg.h"
#include "my_string.h"
#include "msg_creator.h"

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

  if (!p->url)
  {
    char host[HOST_LEN];
    char service[SERVICE_LEN];
    get_peers_url(p->sa, host, service);
    char *tmp = concat(host, ":");
    char *url = concat(tmp, service);
    p->url = url;
    free(tmp);
  }
  char *msg = concat(action, p->url);
  print_log("peers", msg);
  free(msg);
}


int create_epoll(struct list *l_peers)
{
  int epoll_fd = init_epoll();

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
      peer->socket = sock;
      print_peers_connect_log(peer, "connect : ");
    }
  }
  return epoll_fd;
}

struct peer *get_peer_from_sock(struct list *l_peer, int sock)
{
  for (struct node *cur = l_peer->head; cur; cur = cur->next)
  {
    struct peer *p = cur->data;
    if (p->socket == sock)
      return p;
  }
  return NULL;
}

void handle_epoll_event(int epoll_fd, struct list *l_peer)
{
  struct epoll_event *events = malloc(sizeof(struct epoll_event) * 50);
  if (!events)
    err(1, "cannot malloc struct epoll_event");

  int ndfs = 0;
  while ((ndfs = epoll_wait(epoll_fd, events, 50, 1000)) != 0)
  {
    for (int i = 0; i < ndfs; i++)
    {
      int sock = events[i].data.fd;
      struct peer *p = get_peer_from_sock(l_peer, sock);
      if (events[i].events & EPOLLHUP)
      {
        print_peers_connect_log(p, "disconnect: ");
        pop_elt(l_peer, p);
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sock, events + i);
        close(sock);
        printf("ndfs : %d\n", ndfs);
      }
      else if (events[i].events & EPOLLIN)
      {
        char buf[4096];
        ssize_t len = recv(sock, buf, 4096, MSG_TRUNC);
        if (len == -1)
          printf("ERROR RECV\n");
        printf("LEN = %ld\n", len);
        if (len > 0)
          print_msg_log(p, buf, "recv: ");
      }
      else if (events[i].events & EPOLLOUT)
      {
        send_handshake(p, NULL);
        //print_peers_connect_log(p, "epollout");
        if (p->to_send)
        {
          print_msg_log(p, p->to_send, "send: ");
          send(sock, p->to_send, p->msg_len, 0);
        }
        p->to_send = NULL;
      }
    }
  }
}
