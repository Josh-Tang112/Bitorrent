#ifndef HELPERS
#define HELPERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <openssl/sha.h>

#include "data.h"
#include "data_structure.h"

/**
 * malloc, remember to free return value
 * assume url to be of length 20 bytes
 */
char *urlencode(char *url);

/**
 * bitmap is the bitfield of the bitfield message
 * len is the number of bytes in the bitmap
 * return an array so that if ith bit of the bitmap is 1, ith entry
 * of the array is also 1, other wise it'll be zero
 * if there are n pieces of file, this function'll return array of length n * 8 bytes
 * will malloc, don't forget to free
 */
uint8_t *bitfield_to_bytearray(uint8_t *bitmap, unsigned int len);
/**
 * convert byte array into bitfield
 * an inverse function of bitfield_to_bytearray()
 * will malloc, don't forget to free
 */
uint8_t *bytearray_to_bitfield(uint8_t *byte_array, unsigned int len);

/**
 * close the associated socket and free associated memories
 * return 1 if succeed, 0 if p is NULL
*/
int kill_peer(struct peer *p, struct pollfd *fds, int fdnum);

/**
 * return 1 if fdnum will change, 0 if not
*/
int add_fd(struct pollfd *fds, int fdnum, int sock);

/**
 * connect to the list of peers provided
 * return 0 if could not connect to any peer, 0 if not
*/
int connect_to_peer_list(struct peer_list *list, Tracker_response tracker_resp, 
        Meta metainfo, struct pollfd *fds, int *fdnum, uint8_t *handshake);

#endif
