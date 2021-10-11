#ifndef PARSER_H
#define PARSER_H

#include "protocol.h"
#include <string.h>

typedef struct request_text{
    char* (*handler)(game_server*, char*, requester_info);
    char* text;
}request_text;

#endif