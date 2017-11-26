AM_CFLAGS = -Wall -Werror -Wextra -std=c99 -pedantic
bin_PROGRAMS = my-bittorrent
my_bittorrent_SOURCES = src/dictionary.c \
												src/dictionary.h \
												src/my_bittorrent.c \
												src/my_string.c \
												src/my_string.h \
												src/tracker_helper.c \
												src/parsing.c \
												src/parsing.h \
												src/my_bittorrent.h \
												src/list/list.h \
												src/list/list.c \
												src/list/op_list.c \
												src/list/op_list2.c \
												src/print_json.c \
												src/print_json.h \
												src/get_tracker.c \
												src/encode.c \
												src/encode.h \
												src/dump_peers.c \
												src/sock_list.c \
												src/print_log.c \
												src/print_log.h \
												src/my_string.h \
												src/connect_peers.c \
												src/connect_peers.h \
												src/print_msg.c \
												src/print_msg.h \
												src/dictionary_helper.c \
												src/msg_creator.c \
												src/msg_creator.h \
												src/int_utils.c \
												src/int_utils.h \
												src/message_handler.c \
												src/check_piece.c \
												src/piece_write.c

EXTRA_DIST = tests/test_suite.sh \
						 tests/torrents/*.torrent

check-local:
	$(srcdir)/tests/test_suite.sh
