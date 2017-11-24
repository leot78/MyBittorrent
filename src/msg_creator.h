#ifndef MSG_CREATOR_H
# define MSG_CREATOR_H

enum type_simple_msg
{
  CHOKE = 0,
  UNCHOKE = 1,
  INTEREST = 2,
  NOT_INTEREST = 3
};

void send_handshake(struct peer *p, unsigned char *info_hash);
void send_simple_msg(struct peer *p, enum type_simple_msg type);
void send_have(struct peer *p, size_t piece_index);
void send_bitfield(struct peer *p, size_t len, char *bitfield);
void send_request(struct peer *p, size_t index, size_t begin, size_t length);
void send_piece(struct peer *p, size_t index, size_t begin,
                struct block *block);

#endif /* !MSG_CREATOR_H */
