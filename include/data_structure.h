#ifndef __STRUCTURE__
#define __STRUCTURE__

#include <stdio.h>
#include <stdint.h>
#include <time.h>

struct download_state
{
    uint8_t info_hash[20];
    size_t piece_length;
    size_t file_length;
    unsigned int num_piece;
    size_t *downloaded; // an array, for each entry stores the amount of data downloaded
    uint8_t *hash;      // holds the list of 20 bytes SHA1 hash for each pieces
    char **file;
};

struct download_list
{
    struct download_list *next;
    struct download_state *state;
};

typedef enum State
{
    HANDSHAKE,
    MESSAGE
} State;

struct peer
{
    uint8_t id[20];          // optional
    uint8_t am_interested;   // 0 or 1
    uint8_t peer_interested; // 0 or 1
    uint8_t choking;         // 0 or 1
    uint8_t peer_choking;    // 0 or 1
    time_t last_time_msg_sent;
    time_t last_time_msg_recv;
    uint32_t bytes_recv; // for 20 sec
    time_t last_time_rate_calculated;
    uint8_t file_hash[20];
    uint8_t *have; // an array, if have[i] = 1, peer has ith piece of the data
    uint32_t ip;
    uint16_t port; // in network order
    int sock;
    State state;
};

struct peer_list
{
    struct peer_list *next;
    struct peer *p;
};

enum MessageID
{
    CHOKE = 0,
    UNCHOKE = 1,
    INTERESTED = 2,
    NOT_INTERESTED = 3,
    HAVE = 4,
    BITFIELD = 5,
    REQUEST = 6,
    PIECE = 7,
    CANCEL = 8,
    PORT = 9
};

struct pre_request_info
{
    uint32_t piece_index;
    uint32_t begin;
    uint32_t length;
};

struct request
{
    uint32_t index;
    uint32_t begin;
    uint32_t len;
};

struct request_queue
{
    struct request_queue *next;
    struct peer *p;
    struct request req;
};

#pragma pack(1)
struct request_send
{
    uint32_t length;
    uint8_t msgID;
    uint32_t index;
    uint32_t begin;
    uint32_t len;
};

#pragma pack(1)
struct statusChange
{
    uint32_t length;
    uint8_t msgId;
};

struct have
{
    uint8_t msgID;
    uint8_t piece_index;
};

#pragma pack(1)
struct have_send
{
    uint32_t length;
    uint8_t msgID;
    uint32_t piece_index;
};

typedef struct Meta
{
    uint8_t info_hash[20];   // SHA-1 hash of info field of metafile
    uint8_t *piece_hash;     // Pieces field of info dict in metafile. Null terminated.
    unsigned int num_pieces; // Number of pieces (20 byte hash values) in the piece_hash field
    uint32_t piece_length;   // Piece length field of info dict in metafile
    uint32_t length_of_file; // Length of file. We do not need to support multiple files
} Meta;

// Used to send a request to the tracker.
typedef struct Tracker_request
{
    char *announce_url;
    uint16_t port_client;
    char *peer_id;
    uint8_t info_hash[21];
    size_t uploaded;
    size_t downloaded;
    size_t left;
    int compact;
    char event[10]; // One of started, stopped or completed
    time_t last_send;
} Tracker_request;

struct tracker_peer
{
    struct sockaddr_in addr;
    char *peer_id;
};

typedef struct Tracker_response
{
    unsigned int interval;
    unsigned int complete;
    unsigned int incomplete;
    char *tracker_id;
    unsigned int num_peers;
    struct tracker_peer *peers; // Array of peers
} Tracker_response;

#endif