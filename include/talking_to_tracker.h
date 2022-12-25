#ifndef _TALKING_TO_TRACKER_H
#define _TALKING_TO_TRACKER_H

#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include "data_structure.h"

int connect_to_tracker(char *name, int port_tracker, struct sockaddr_in my_addr);
void parse_tracker_response(struct bencode *tracker_response, Tracker_response *ret);
struct bencode *get_info_tracker(int sd);
struct bencode *read_torrent_file(char *file);
Meta parse_metafile(struct bencode *metafile);
struct bencode *send_tracker_request(Tracker_request *tr, struct sockaddr_in my_addr);
Tracker_request init_tracker_request(struct bencode *metafile, uint16_t port_client);
void change_event_to_stopped(Tracker_request *tr);
void change_event_to_completed(Tracker_request *tr);

#endif
