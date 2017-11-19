#ifndef CONNECT_PERS_H
# define CONNECT_PERS_H

int create_epoll(struct list *l_peers, int *arr_sock);
void handle_epoll_event(int epoll_fd, int *arr_sock, struct list *l_peer);

#endif /* !CONNECT_PERS_H */
