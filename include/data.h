#ifndef DATA
#define DATA

#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <inttypes.h>

#include "data_structure.h"
#include "hash.h"
#include "helpers.h"

/**
 * init request queue
*/
struct request_queue *init_queue();

/**
 * Cannot send 2 identical request to same peer
 * Will not add if the above condition is violated 
 * return 1 if added successfully, 0 if not
*/
int add_to_queue(struct request_queue *queue, struct peer *p, 
        uint32_t index, uint32_t begin, uint32_t len);

/**
 * return 1 if such request is in queue, 0 if not
*/
int is_in_queue(struct request_queue *queue, struct peer *p, 
        uint32_t index, uint32_t begin);

/**
 * remove the according request from the queue
 * return 1 if successful, 0 if could not find such request
*/
int remove_queue(struct request_queue *queue, struct peer *p, 
        uint32_t index, uint32_t begin);

/**
 * Use this to initialize the list of download states
*/
struct download_list *init_download_list();
/**
 * add state to the list
 * MUST provide info hash and hash of the pieces
 * make sure the length of piece_hash is multiple of 20
*/
void add_download_state(struct download_list *list, uint8_t *info_hash, 
        unsigned int num_pieces, unsigned int piece_length, 
        uint8_t *piece_hash, size_t file_length);
/**
 * l1 and l2 are 20 byte hash
 * return 1 if equal, 0 if not
*/
int same_20byte(uint8_t *l1, uint8_t *l2);
/**
 * find the file that has the same info hash
*/
struct download_state *find_file_by_hash(struct download_list *list, uint8_t hash[20]);
/**
 * Use this to free the initialized list
 * Free everything that's in the list and the list
*/
void free_download_list(struct download_list *list);

/**
 * Use this to initialize the list of peers
*/
struct peer_list *init_peer_list();
/**
 * Use this to free the initialized list
 * Free everything that's in the list and the list
*/
void free_peer_list(struct peer_list *list);

/**
 * return the peer that contains the sock, NULL if there isn't any
*/
struct peer* has_sock(struct peer_list *list, int sock);
/**
 * Do NOT call btohs() on port when you pass it as argument
 * Assume port is in big endian
 * return the peer that has the ip and port, NULL if there isn't any 
*/
struct peer* connected(struct peer_list *list, uint32_t ip, uint16_t port);

/**
 * Add peer with the provided info to the list
 * id is optional, pass in NULL if not provided
 * file_hash is mandatory will not add if file_hash is NULL
 * Make sure port is little endian
 * Will NOT add if there is already peer in the list that have the same ip and port
 * Choking and peer choking is 1
 * interested and peer_insterested is 0
 * num_piece is number of pieces the file has
 * All other fields are set to 0
 * return the pointer to the peer that's just added, NULL if failed
*/
struct peer* add_peer(struct peer_list *list, uint8_t *id, 
        uint8_t *file_hash, uint32_t ip, uint16_t port, unsigned int num_pieces);

/**
 * remove peer with the corresponding socket from the list
 * does NOT free the peer
 * return the peer that's being removed, NULL if such peer cannot be found
*/
struct peer* del_peer_by_sock(struct peer_list *list, int sock);
/**
 * remove peer with the corresponding ip and port from the list
 * assume port is in BIG endian
 * does NOT free the peer
 * return the peer that's being removed, NULL if such peer cannot be found
*/
struct peer* del_peer_by_ip(struct peer_list *list, uint32_t ip, uint16_t port);

void update_send_timer(struct peer *p);
void update_recv_timer(struct peer *p);
void update_send_timer_sock(struct peer_list *list, int sock);
void update_recv_timer_sock(struct peer_list *list, int sock);

/**
 * sock is the socket where the message come from 
*/
void handle_peer_keepalive(struct peer *p);
void handle_peer_choke(struct peer *p);
void handle_peer_unchoke(struct peer *p);
void handle_peer_interested(struct peer *p);
void handle_peer_uninterested(struct peer *p);
/**
 * index is zero based
*/
void handle_peer_have(struct peer *p, int index);
/**
 * bitfield is the <bitfield> part of the message
 * len is the number of bytes in bitfield
 * return 0 if peer.have is already initialized, 1 if not
*/
void handle_peer_bitfield(struct peer *p, uint8_t *bitfield, 
                unsigned int len, unsigned int num_piece);
/**
 * if we are not choking the respective peer, 
 * and has the respective piece,
 * will send the requested piece
 * assume index, begin, length are in little endian
 * return:
 * -1 if no piece is sent because we are choking peer,
 * -2 if we do not have enough what peer requested
 * -3 if send() failed
 * 1 if the requested piece is sent
*/
int handle_peer_request(struct peer *p, struct download_list *files,
        uint32_t index, uint32_t begin, uint32_t length);
/**
 * length is the number of bytes in block
 * add the data to the files of the respective download_state
 * check the hash of the piece if the piece is fully downloaded
 * if the hash does not match, will delete the entire piece
 * return 0 if the hash does not match, 1 if match
*/
int handle_peer_piece(struct peer *p, struct download_list *files,
        uint32_t index, uint32_t begin, uint8_t *block, size_t length);

/**
 * return 1 if all pieces are downloaded, 0 if not
*/
int all_downloaded(struct download_state *state);
#endif