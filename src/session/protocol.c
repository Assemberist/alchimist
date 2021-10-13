#include "parser.h"
#include "session.h"

#define WRONG_REQUEST "Error: unknown request. Fuck you!"

request_text request_handlers[] = {
    {_ADMIN_KICK_GUEST, "I command to kick this guest"},
    {_ADMIN_KICK_ALL_GUESTS, "I command to kick all guests"},
    {_ADMIN_RENAME_GUEST, ""},
    {_ADMIN_KICK_CLIENT_FROM_SESSION, "I command to kick this gay"},
    {_ADMIN_KICK_CLIENT_FROM_SERVER, "I command this gay get out from my server"},
    {_ADMIN_KICK_ALL_CLIENTS_FROM_SESSION, "I command kick all gays"},
    {_ADMIN_KICK_ALL_CLIENTS_FROM_SERVER, "I command all gays get out from my server"},
    {_ADMIN_RENAME_CLIENT, "I command to name he as"},
    {_ADMIN_KILL_SESSION, "I command all go fuck yourself"},
    {_ADMIN_EXTERMINANTUS, "I command to execute exterminatus on this server. This resourse should be burned in a cleansing fire"},

    {_SET_NAME, "Now i`m"},
    {_LIST_SESSIONS, "List sessions"},
    {_LIST_LIBRARYES, "List libs"},
    {_CONNECT_SESSION, "Add me"},
    {_CREATE_SESSION, "Create session"},
    {_SUMM_ELEMENTS, "Summ"},
    {_LIST_GAMERS, "Who are there"},
    {_LEAVE, "I`m out"},
    {_WRONG_REQUEST, NULL}
};

void handle_request(game_server* game, char* src, requester_info info){
    strtok(src, ":");
    request_text* ptr;
    for(ptr = request_handlers; ptr->text; ptr++)
        if(!strcmp(src, ptr->text)) break;

    ptr->handler(game, strtok(NULL, ""), info);
}

int get_requester(game_server* game, requester_info info){
    return (
        info.guest.is_guest ? 
            game->guests[info.guest.num].client_socket : 
            game->sessions[info.client.session_num].gamers[info.client.id].data.client_socket
    );
}

void _ADMIN_KICK_GUEST(game_server* game, char* src, requester_info info){}
void _ADMIN_KICK_ALL_GUESTS(game_server* game, char* src, requester_info info){}
void _ADMIN_RENAME_GUEST(game_server* game, char* src, requester_info info){}
void _ADMIN_KICK_CLIENT_FROM_SESSION(game_server* game, char* src, requester_info info){}
void _ADMIN_KICK_CLIENT_FROM_SERVER(game_server* game, char* src, requester_info info){}
void _ADMIN_KICK_ALL_CLIENTS_FROM_SESSION(game_server* game, char* src, requester_info info){}
void _ADMIN_KICK_ALL_CLIENTS_FROM_SERVER(game_server* game, char* src, requester_info info){}
void _ADMIN_RENAME_CLIENT(game_server* game, char* src, requester_info info){}
void _ADMIN_KILL_SESSION(game_server* game, char* src, requester_info info){}
void _ADMIN_EXTERMINANTUS(game_server* game, char* src, requester_info info){}
void _SET_NAME(game_server* game, char* src, requester_info info){}
void _LIST_GAMERS(game_server* game, char* src, requester_info info){}
void _LIST_SESSIONS(game_server* game, char* src, requester_info info){}
void _LIST_LIBRARYES(game_server* game, char* src, requester_info info){}
void _CONNECT_SESSION(game_server* game, char* src, requester_info info){}
void _CREATE_SESSION(game_server* game, char* src, requester_info info){}
void _SUMM_ELEMENTS(game_server* game, char* src, requester_info info){}
void _LEAVE(game_server* game, char* src, requester_info info){}

void _WRONG_REQUEST(game_server* game, char* src, requester_info info){
    int requester = get_requester(game, info);
    write(requester, WRONG_REQUEST, sizeof(WRONG_REQUEST));
}