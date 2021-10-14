#include "parser.h"
#include "session.h"
#include <stdlib.h>
#include <string.h>

request_text request_handlers[] = {
    {_ADMIN_KICK_GUEST, "I command to kick this guest"},
    {_ADMIN_KICK_ALL_GUESTS, "I command to kick all guests"},
    {_ADMIN_RENAME_GUEST, "I command to name he as"},
    {_ADMIN_KICK_CLIENT_FROM_SESSION, "I command to kick this gay"},
    {_ADMIN_KICK_CLIENT_FROM_SERVER, "I command this gay get out from my server"},
    {_ADMIN_KICK_ALL_CLIENTS_FROM_SESSION, "I command kick all gays"},
    {_ADMIN_KICK_ALL_CLIENTS_FROM_SERVER, "I command all gays get out from my server"},
    {_ADMIN_RENAME_CLIENT, "I command to name that gay as"},
    {_ADMIN_KILL_SESSION, "I command all go fuck yourself"},
    {_ADMIN_EXTERMINANTUS, "I command to execute exterminatus on this server. This resourse should be burned in a cleansing fire"},

    {_SET_NAME, "Now i`m"},
    {_LIST_LOBBY, "Who connected"},
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

int get_requester_socket(game_server* game, requester_info info){
    return (
        info.guest.is_guest ? 
            game->guests[info.guest.num].client_socket : 
            game->sessions[info.client.session_num].gamers[info.client.id].data.client_socket
    );
}

char** get_requester_name(game_server* game, requester_info info){
    return (
        info.guest.is_guest ? 
            &game->guests[info.guest.num].login : 
            &game->sessions[info.client.session_num].gamers[info.client.id].data.login
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

void _SUMM_ELEMENTS(game_server* game, char* src, requester_info info){}
void _CREATE_SESSION(game_server* game, char* src, requester_info info){}
void _CONNECT_SESSION(game_server* game, char* src, requester_info info){}

// no check
void _LIST_SESSIONS(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);
    char buffer[200] = "Session info:";
    char* ptr = buffer + sizeof("Session info:")-1;
    size_t i;
    for(i = 0; i < max_sessions; i++){
        if(game->sessions[i].path){
            *(ptr++) = game->sessions[i].session_id + '0';
            *(ptr++) = ' ';
            strcpy(ptr, game->sessions[i].path);
            strcat(ptr, " [");
            char* status;
            switch(game->sessions[i].gamer_count){
                case 0: status = "Abandoned"; break;
                case gamer_capacity: status = "All busy"; break;
                default: status = "Free";
            }
            strcat(ptr, status);
            strcat(ptr, "]");
            write(requester_socket, buffer, strlen(buffer)+1);
        }
    }
    write(requester_socket, MSG_DONE, sizeof(MSG_DONE));
}

//no check
void _LIST_LIBRARYES(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);
    char buffer[200] = "Lib info:";
    char* ptr = buffer + sizeof("Lib info:")-1;
    size_t i = game->library_count;
    while(i--){
        strcpy(ptr, game->libraryes[i]);
        write(requester_socket, buffer, strlen(buffer)+1);
    }
    write(requester_socket, MSG_DONE, sizeof(MSG_DONE));
}

// no check
void _LIST_GAMERS(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);

    if(!src) 
        write(requester_socket, MSG_NO_SESSION_ID, sizeof(MSG_NO_SESSION_ID));
    else if(src[0] > '7' || src[0] < '0') 
        write(requester_socket, MSG_WRONG_SESSION_ID, sizeof(MSG_WRONG_SESSION_ID));
    else{
        char buffer[50] = "Gamers info:";
        char* ptr = buffer + sizeof("Gamers info:") - 1;

        size_t i;
        for(i = 0; i < game->sessions[src[0]].gamer_count; i++){
            strcpy(ptr, game->sessions[src[0]].gamers[i].data.login ? game->sessions[src[0]].gamers[i].data.login : "Anonimus");
            write(requester_socket, buffer, strlen(buffer)+1);
        }
    }
    write(requester_socket, MSG_DONE, sizeof(MSG_DONE));
}

void _SET_NAME(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);
    const char* answer;

    char buffer[40];
    if(!src) answer = MSG_NO_NAME;
    else{
        size_t len = strlen(src);
        if(len > 30) answer = MSG_LARGE_NAME;
        else{
            char** name = get_requester_name(game, info);
            if(*name) free(*name);
            *name = (char*)malloc(len+1);
            strcpy(*name, src);
            
            strcpy(buffer, "Hello: ");
            strcat(buffer, src);
            answer = buffer;
        }
    }
    write(requester_socket, answer, strlen(answer)+1);
}

void _LIST_LOBBY(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);

    char buffer[50] = "Lobby info:";
    char* ptr = buffer + sizeof("Lobby info:") - 1;

    size_t i;
    for(i = 0; i < game->guest_count; i++){
        strcpy(ptr, game->guests[i].login ? game->guests[i].login : "Anonimus");
        write(requester_socket, buffer, strlen(buffer)+1);
    }
    write(requester_socket, MSG_DONE, sizeof(MSG_DONE));
}

// no check
void _LEAVE(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);
    char buffer[100];
    char* name = *get_requester_name(game, info);
    strcpy(buffer, name ? name : "Anonimus");
    strcat(buffer, " leave the game");

    if(!info.guest.is_guest){
        gamer_leave(game, info.client.session_num, info.client.id);
        size_t i = game->sessions[info.client.session_num].gamer_count;
        while(i--)
            write(game->sessions[info.client.session_num].gamers[i].data.client_socket, buffer, strlen(buffer)+1);
    }
    write(requester_socket, MSG_LEAVE, sizeof(MSG_LEAVE));
}

void _WRONG_REQUEST(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);
    write(requester_socket, MSG_WRONG_REQUEST, sizeof(MSG_WRONG_REQUEST));
}