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
#include <string.h>

#include "connect_peers.h"
#include "my_bittorrent.h"
#include "list/list.h"
#include "print_log.h"
#include "print_msg.h"
#include "my_string.h"
#include "msg_creator.h"

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
  int epoll_fd = epoll_create(1);
  if (epoll_fd == -1)
    err(1, "cannot create epoll");

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

size_t get_msg_len(void *msg)
{
  char *tmp = msg;
  if (tmp[0] == 0x13)
    return 68;
  struct raw_mess *rm = msg;
  return ntohl(rm->len) + 4;
}

void get_all_msg(ssize_t len, size_t mess_len, char *msg, struct peer *p)
{
  char buf[MAX_MSG_LEN];
  size_t get_len = len;
  while (get_len != mess_len)
  {
    len = recv(p->socket, buf, mess_len - get_len, MSG_WAITALL);
    if (len >= 1)
    {
      memcpy(msg + get_len, buf, len);
      get_len += len;
    }
  }
}


void parse_buffer(ssize_t len, char *buf, struct list *l_peer, struct peer *p)
{
  ssize_t index = 0;
  while (index < len)
  {
    char *cur = buf + index;
    ssize_t msg_len = get_msg_len(cur);
    if (msg_len > len - index)
    {
      char msg[MAX_MSG_LEN];
      memcpy(msg, buf, len - index);
      get_all_msg(len - index, msg_len, msg, p);
      print_msg_log(p, msg, "recv: ");
      message_handler(msg, p, l_peer);
      return;
    }
    if (msg_len > 4)
    {
      print_msg_log(p, cur, "recv: ");
      message_handler(cur, p, l_peer);
    }
    index += msg_len;
  }
}

void disconnect_peer(struct peer *p, int epoll_fd, struct list *l_peer,
                     struct epoll_event *event)
{
  print_peers_connect_log(p, "disconnect: ");
  pop_elt(l_peer, p);
  epoll_ctl(epoll_fd, EPOLL_CTL_DEL, p->socket, event);
  free_peer(p);
}

void recv_from_peer(struct peer *p, int sock, struct list *l_peer)
{
  char buf[MAX_MSG_LEN];
  ssize_t len = recv(sock, buf, MAX_MSG_LEN, 0);
  if (len == -1)
    printf("ERROR RECV\n");
  if (len > 0)
    parse_buffer(len, buf, l_peer, p);
}

void send_peer(struct peer *p, int sock)
{
  char *msg = pop_front(p->q_send);
  print_msg_log(p, msg, "send: ");
  size_t len = get_msg_len(msg);
  send(sock, msg, len, 0);
  free(msg);
}

void handle_epoll_event(int epoll_fd, struct list *l_peer)
{
  struct epoll_event events[MAX_EVENTS];
  make_all_handshake(l_peer);

  int ndfs = 0;
  while ((ndfs = epoll_wait(epoll_fd, events, MAX_EVENTS, 1000)) != 0
          && !g_client.finish)
  {
    for (int i = 0; i < ndfs; i++)
    {
      int sock = events[i].data.fd;
      struct peer *p = get_peer_from_sock(l_peer, sock);
      if (events[i].events & EPOLLHUP)
        disconnect_peer(p, epoll_fd, l_peer, events + i);
      else if (events[i].events & EPOLLIN)
        recv_from_peer(p, sock, l_peer);
      else if (events[i].events & EPOLLOUT && p->q_send->size)
        send_peer(p, sock);
    }
  }
  for (int i = 0; i < ndfs; i++)
  {
    int sock = events[i].data.fd;
    struct peer *p = get_peer_from_sock(l_peer, sock);
    disconnect_peer(p, epoll_fd, l_peer, events + i);
  }
  close(epoll_fd);
}
