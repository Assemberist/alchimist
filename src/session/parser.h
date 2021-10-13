#ifndef PARSER_H
#define PARSER_H

#include "protocol.h"
#include <string.h>

typedef struct request_text{
    void (*handler)(game_server*, char*, requester_info);
    char* text;
}request_text;

void _ADMIN_KICK_GUEST(game_server*, char*, requester_info);
void _ADMIN_KICK_ALL_GUESTS(game_server*, char*, requester_info);
void _ADMIN_RENAME_GUEST(game_server*, char*, requester_info);
void _ADMIN_KICK_CLIENT_FROM_SESSION(game_server*, char*, requester_info);
void _ADMIN_KICK_CLIENT_FROM_SERVER(game_server*, char*, requester_info);
void _ADMIN_KICK_ALL_CLIENTS_FROM_SESSION(game_server*, char*, requester_info);
void _ADMIN_KICK_ALL_CLIENTS_FROM_SERVER(game_server*, char*, requester_info);
void _ADMIN_RENAME_CLIENT(game_server*, char*, requester_info);
void _ADMIN_KILL_SESSION(game_server*, char*, requester_info);
void _ADMIN_EXTERMINANTUS(game_server*, char*, requester_info);
void _LIST_GAMERS(game_server*, char*, requester_info);
void _SET_NAME(game_server*, char*, requester_info);
void _LIST_SESSIONS(game_server*, char*, requester_info);
void _LIST_LIBRARYES(game_server*, char*, requester_info);
void _CONNECT_SESSION(game_server*, char*, requester_info);
void _CREATE_SESSION(game_server*, char*, requester_info);
void _SUMM_ELEMENTS(game_server*, char*, requester_info);
void _LEAVE(game_server*, char*, requester_info);
void _WRONG_REQUEST(game_server*, char*, requester_info);

#endif