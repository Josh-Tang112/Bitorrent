#ifndef BITTORRENT_ARG_PARSER_H
#define BITTORRENT_ARG_PARSER_H

#include <inttypes.h>
#include <arpa/inet.h>
#include <argp.h>

struct bittorrent_arguments
{
    char file_path[1024];
    struct sockaddr_in my_address;
};

error_t bittorrent_parser(int key, char *arg, struct argp_state *state);

struct bittorrent_arguments bittorrent_parseopt(int argc, char *argv[]);

#endif
