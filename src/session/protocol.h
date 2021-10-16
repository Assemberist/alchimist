#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "session.h"

void handle_request(game_server* game, char* src, requester_info info);

#endif