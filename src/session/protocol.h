#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "session.h"

typedef union requester_info{
    struct{
        int8_t is_guest:1;
        int8_t num:7;
    }guest;
    struct{
        int8_t is_guest:1;
        int8_t session_num:3;
        int8_t id:4;
    }client;
} requester_info;

char* handle_request(game_server* game, char* src, requester_info info);

#endif