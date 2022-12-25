#ifndef _TALKING_TO_PEER_H
#define _TALKING_TO_PEER_H

#include <math.h>
#include <openssl/sha.h>
#include "helpers.h"
#include "data_structure.h"

void send_keepAlive(struct peer *p);
void send_choke(struct peer *p);
void send_unchoke(struct peer *p);
void send_interested(struct peer *p);
void send_not_interested(struct peer *p);
void send_have(struct peer *p, uint32_t piece_index);
void send_bitfield(struct peer *p, uint8_t *data, uint32_t len);
void send_request(struct peer *p, uint32_t rq_index, uint32_t rq_begin, uint32_t rq_len);
// void send_piece(struct peer *p, char *file_piece, uint32_t piece_index);

uint8_t getIncomingMsgID(int sock);
char *generate_peer_id();
uint8_t *createHandshake(char *peer_id, char *hash);
int check_handshake(uint8_t *handshake, uint8_t *peer_handshake, int recvLen);

struct pre_request_info *check_needed_pieces(struct download_state *state);

// will malloc, remember to free the returned value
uint8_t *create_bitfield(struct download_state *state);
int contains_missing_piece(struct peer *pr, struct download_state *state);

#endif