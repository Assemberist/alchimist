#ifndef PARSER_H
#define PARSER_H

#include "protocol.h"
#include <string.h>

typedef struct request_text{
    void (*handler)(game_server*, char*, requester_info);
    char* text;
}request_text;

#define MSG_WRONG_REQUEST "Error: unknown request. Fuck you!"
#define MSG_LARGE_NAME "Error: Too large name. Fuck you!"
#define MSG_NO_NAME "Error: Name does not setted. Fuck you!"
#define MSG_LEAVE "Get out from my game!"
#define MSG_NO_SESSION_ID "Error: session number does not setted. Fuck you!"
#define MSG_WRONG_SESSION_ID "Error: Sessions can be from 0 to 7, you, moron! Fuck you!"
#define MSG_WRONG_CLIENT_ID "Error: There is no that gay on server. Fuck you!"
#define MSG_ADMIN_KILL "Admin: Go to gazenwagen!"
#define MSG_ADMIN_RENAME "Admin: Your name now"
#define MSG_SESSION_EXIST "Warn: There is same session: "
#define MSG_EXTERMINATED "Holy Emperor, this world is exterminated. But what low cost is that tragedy for the humanity."
#define MSG_DONE "Done"

void _ADMIN_KICK_GUEST(game_server*, char*, requester_info);
void _ADMIN_KICK_ALL_GUESTS(game_server*, char*, requester_info);
void _ADMIN_RENAME_GUEST(game_server*, char*, requester_info);
void _ADMIN_KICK_CLIENT(game_server*, char*, requester_info);
void _ADMIN_KICK_ALL_CLIENTS_FROM_SESSION(game_server*, char*, requester_info);
void _ADMIN_KICK_ALL_CLIENTS_FROM_SERVER(game_server*, char*, requester_info);
void _ADMIN_RENAME_CLIENT(game_server*, char*, requester_info);
void _ADMIN_EXTERMINANTUS(game_server*, char*, requester_info);
void _LIST_LOBBY(game_server* game, char* src, requester_info info);
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