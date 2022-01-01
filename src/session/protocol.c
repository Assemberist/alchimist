#include "parser.h"
#include "session.h"
#include <stdlib.h>
#include <string.h>

request_text request_handlers[] = {
    {_ADMIN_KICK_GUEST, "I command to kick this guest"},
    {_ADMIN_KICK_ALL_GUESTS, "I command to kick all guests"},
    {_ADMIN_RENAME_GUEST, "I command to name he as"},
    {_ADMIN_KICK_CLIENT, "I command this gay get out from my server"},
    {_ADMIN_KICK_ALL_CLIENTS_FROM_SESSION, "I command kick all gays"},
    {_ADMIN_KICK_ALL_CLIENTS_FROM_SERVER, "I command all gays get out from my server"},
    {_ADMIN_RENAME_CLIENT, "I command to name that gay as"},
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

////////////////////////////////////////////////////////////////////////////////////////
// service functions                                                                ////
////////////////////////////////////////////////////////////////////////////////////////

void handle_request(game_server* game, char* src, requester_info info){
    strtok(src, ":");
    request_text* ptr;
    for(ptr = request_handlers; ptr->text; ptr++)
        if(!strcmp(src, ptr->text)) break;

    ptr->handler(game, strtok(NULL, ":"), info);
}

int check_session(char* src, int sock){
    if(!src)
        write(sock, MSG_NO_SESSION_ID, sizeof(MSG_NO_SESSION_ID));
    else if(src[0] > '7' || src[0] < '0') 
        write(sock, MSG_WRONG_SESSION_ID, sizeof(MSG_WRONG_SESSION_ID));
    else return 1;

    return 0;
}

size_t check_client_id(char* src, int sock){
    if(!src){
        write(sock, MSG_NO_NAME, sizeof(MSG_NO_NAME));
        return gamer_capacity;
    }

    size_t num = atoi(src);

    if(num >= gamer_capacity){
        write(sock, MSG_WRONG_CLIENT_ID, sizeof(MSG_WRONG_CLIENT_ID));
        return gamer_capacity;
    }

    return num;
}

void move_client(game_server* game, requester_info from, requester_info to){
    int requester_socket = get_requester_socket(game, from);
    if(game->sessions[to.client.session_num].gamer_count == gamer_capacity){
        write(requester_socket, MSG_SESSION_BUSY, sizeof(MSG_SESSION_BUSY));
        return;
    }

    if(from.client.is_guest == 0)
        if(from.client.session_num == to.client.session_num){
            write(requester_socket, MSG_ALREADY_HERE, sizeof(MSG_ALREADY_HERE));
            return;
        }

    to.client.id = game->sessions[to.client.session_num].gamer_count;

    new_gamer(game, from, to);
    _LEAVE(game, (char*)1, from);
}

element* check_element(char* src, int sock, token* worterbuch){
    if(!src){
        write(sock, MSG_NO_ELEMENT, sizeof(MSG_NO_ELEMENT));
        return NULL;
    }

    element* element = find_element(src, worterbuch);

    if(!element){
        char buffer[200] = "Error: There is no \"";
        strcat(src, get_el_name(element));
        strcat(src, "\" element");
        write(sock, buffer, strlen(buffer+1));
    }

    return element;
}

////////////////////////////////////////////////////////////////////////////////////////
// request handlers                                                                 ////
////////////////////////////////////////////////////////////////////////////////////////

void _ADMIN_KICK_GUEST(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);

    if(!src){
        write(requester_socket, MSG_NO_NAME, sizeof(MSG_NO_NAME));
        return;
    }

    size_t num = atoi(src);
    requester_info jackass;
    jackass.guest.is_guest = 1;
    jackass.guest.num = num;

    write(game->guests[num].client_socket, MSG_ADMIN_KILL, sizeof(MSG_ADMIN_KILL));
    _LEAVE(game, NULL, jackass);

    write(requester_socket, MSG_DONE, sizeof(MSG_DONE));
}

void _ADMIN_KICK_ALL_GUESTS(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);

    size_t i = game->guest_count;
    while(i--){
        requester_info jackass;
        jackass.guest.is_guest = 1;
        jackass.guest.num = i;

        write(game->guests[i].client_socket, MSG_ADMIN_KILL, sizeof(MSG_ADMIN_KILL));
        _LEAVE(game, NULL, jackass);
    }

    write(requester_socket, MSG_DONE, sizeof(MSG_DONE));
}

void _ADMIN_RENAME_GUEST(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);

    if(!src){
        write(requester_socket, MSG_NO_NAME, sizeof(MSG_NO_NAME));
        return;
    }

    size_t num = atoi(src);

    requester_info jackass;
    jackass.guest.is_guest = 1;
    jackass.guest.num = num;

    src = strtok(NULL, ":");

    if(!src){
        write(requester_socket, MSG_NO_NAME, sizeof(MSG_NO_NAME));
        return;
    }

    write(game->guests[num].client_socket, MSG_ADMIN_RENAME, sizeof(MSG_ADMIN_RENAME));
    _SET_NAME(game, src, jackass);

    write(requester_socket, MSG_DONE, sizeof(MSG_DONE));
}

// no check
void _ADMIN_KICK_CLIENT(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);

    if(!check_session(src, requester_socket)) return;

    char* client_text = strtok(NULL, ":");
    size_t client_id = check_client_id(src, requester_socket);
    if(client_id == gamer_capacity) return;

    requester_info jackass;
    jackass.client.is_guest = 0;
    jackass.client.session_num = *src - '0';
    jackass.client.id = client_id;

    write(get_requester_socket(game, jackass), MSG_ADMIN_KILL, sizeof(MSG_ADMIN_KILL));
    _LEAVE(game, NULL, jackass);

    write(requester_socket, MSG_DONE, sizeof(MSG_DONE));    
}

// no check
void _ADMIN_KICK_ALL_CLIENTS_FROM_SESSION(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);

    if(!check_session(src, requester_socket)) return;
    session* session_ptr = game->sessions + *src - '0';

    requester_info jackass;
    jackass.client.is_guest = 0;
    jackass.client.session_num = *src - '0';

    size_t i;
    for(i = 0; i < session_ptr->gamer_count; i++){
        jackass.client.id = i;

        write(session_ptr->gamers[i].data.client_socket, MSG_ADMIN_KILL, sizeof(MSG_ADMIN_KILL));
        _LEAVE(game, NULL, jackass);
    }

    char* del = strtok(src, ":");
    if(del) if(*del == 'd') {
        session_ptr->path = NULL;
        dispose_library(&session_ptr->lib);
        session_ptr->max_desc = 0;
    }

    write(requester_socket, MSG_DONE, sizeof(MSG_DONE));    
}

// no check
void _ADMIN_KICK_ALL_CLIENTS_FROM_SERVER(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);

    char buffer[] = " ";
    size_t i = max_sessions;
    while(i--){
        buffer[0] = '0' + i;
        _ADMIN_KICK_ALL_CLIENTS_FROM_SESSION(game, buffer, info);
    }

    write(requester_socket, MSG_DONE, sizeof(MSG_DONE));    
}

void _ADMIN_RENAME_CLIENT(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);

    if(!check_session(src, requester_socket)) return;

    size_t session_num = *src - '0';
    src = strtok(NULL, ":");
    size_t client_num = check_client_id(src, requester_socket);
    if(client_num == gamer_capacity) return;

    requester_info jackass;
    jackass.client.is_guest = 0;
    jackass.client.session_num = session_num;
    jackass.client.id = client_num;

    char* name = strtok(NULL, "");
    if(!name){
        write(requester_socket, MSG_NO_NAME, sizeof(MSG_NO_NAME));
        return;
    }

    _SET_NAME(game, name, jackass);
    write(requester_socket, MSG_DONE, sizeof(MSG_DONE));
}

void _ADMIN_EXTERMINANTUS(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);

    _ADMIN_KICK_ALL_GUESTS(game, NULL, info);

    size_t i = max_sessions;
    char buffer[] = "0:d";
    while(i--){
        buffer[0] = '0' + i;
        _ADMIN_KICK_ALL_CLIENTS_FROM_SESSION(game, buffer, info);
    }

    game->enough = 1;

    write(requester_socket, MSG_EXTERMINATED, sizeof(MSG_EXTERMINATED));
}

void _SUMM_ELEMENTS(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);

    if(info.guest.is_guest){
        write(requester_socket, MSG_YOU_GUEST, sizeof(MSG_YOU_GUEST));
        return;
    }

    session* party = game->sessions + info.client.session_num;

    element* reagent1 = check_element(src, requester_socket, party->lib.worterbuch);
    src = strtok(NULL, ":");
    element* reagent2 = check_element(src, requester_socket, party->lib.worterbuch);

    if(!(reagent1 && reagent2)) return;

    element* rezult = dualisation(&party->lib, reagent1, reagent2, info.client.session_num);
    if(rezult){
        if(rezult->shortName.is_open == 0){
            rezult->shortName.is_open = 1;

            char buffer[1000] = "Party info: Gamer ";
            char* sas = *get_requester_name(game, info);
            strcat(buffer, sas ? sas : "anonimus");
            strcat(buffer, " open new combination:");
            strcat(buffer, get_el_name(reagent1));
            strcat(buffer, "+");
            strcat(buffer, get_el_name(reagent2));
            strcat(buffer, "=");
            strcat(buffer, get_el_name(rezult));

            size_t len = strlen(buffer)+1;
            size_t i = party->gamer_count;
            while(i--)
                write(party->gamers[i].data.client_socket, buffer, len);

            strcpy(buffer, "Element:");
            strcat(buffer, get_el_name(rezult));

            len = strlen(buffer)+1;
            i = party->gamer_count;
            while(i--)
                write(party->gamers[i].data.client_socket, buffer, len);
        }
        else{
            char buffer[200] = "You received: ";
            strcat(buffer, get_el_name(rezult));
            write(requester_socket, buffer, strlen(buffer)+1);
        }
    }
    else write(requester_socket, "You failed. Try Again.", sizeof("You failed. Try Again."));
}

void _CREATE_SESSION(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);

    if(!src){
        write(requester_socket, "Error: Library required. Fuck you", sizeof("Error: Library required. Fuck you"));
        return;
    }
 
    char* force = strtok(NULL, ":");

    char* answer;
    size_t i;

    size_t lib = game->library_count;
    while(lib--)
        if(!strcmp(game->libraryes[lib], src))
            goto found;

    answer = "Error: No that library. Fuck you!";
    goto exit;

found:

    if(!force){
        i = max_sessions;
        while(i--){
            if(game->libraryes[lib] == game->sessions[i].path){
                char buffer[sizeof(MSG_SESSION_EXIST)] = MSG_SESSION_EXIST;
                buffer[sizeof(MSG_SESSION_EXIST)-1] = '0' + i;
                answer = buffer;
                goto exit;
            }
        }
    }

    i = max_sessions;
    while(i--){
        if(!game->sessions[i].path)
            goto connect;

        if(game->sessions[i].gamer_count == 0){
            dispose_library(&game->sessions[i].lib);
            goto connect;
        }
    }

    answer = "Error: All sessions busy. Fuck you!";
    goto exit;

connect:

    open_session(game, i, game->libraryes[lib]);
    requester_info to;
    to.client.is_guest = 0;
    to.client.id = 0;
    to.client.session_num = i;
    move_client(game, info, to);
    write(requester_socket, "Connected", sizeof("Connected"));

    session* ptr = game->sessions + i;
    char buffer[200] = "Element:";
    char* str = buffer + sizeof("Element:")-1;
    group* libr = ptr->lib.groups;
    size_t gr = ptr->lib.group_count;
    while(gr--){
        size_t el = (libr + gr)->name_count;
        while(el--){
            element* elementus = ((libr + gr)->names + el);
            if(elementus->shortName.is_open){
                strcpy(str, get_el_name(elementus));
                write(requester_socket, buffer, strlen(buffer) + 1);
            }
        }
    }

    answer = MSG_DONE;

exit:

    write(requester_socket, answer, strlen(answer)+1);
}

void _CONNECT_SESSION(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);
    if(!check_session(src, requester_socket)) return;

    session* ptr = game->sessions + *src - '0';

    if(ptr->gamer_count == gamer_capacity){
        write(requester_socket, MSG_SESSION_BUSY, sizeof(MSG_SESSION_BUSY));
        return;
    }

    if(!ptr->path){
        write(requester_socket, "Error: Session is not started. Fuck you!", sizeof("Error: Session is not started. Fuck you!"));
        return;
    }

    requester_info addr;
    addr.client.is_guest = 0;
    addr.client.session_num = *src - '0';

    write(requester_socket, "Connected", sizeof("Connected"));
    move_client(game, info, addr);

    char buffer[200] = "Element:";
    char* str = buffer + sizeof("Element:")-1;
    group* lib = ptr->lib.groups;
    size_t gr = ptr->lib.group_count;
    while(gr--){
        size_t el = (lib + gr)->name_count;
        while(el--){
            element* elementus = ((lib + gr)->names + el);
            if(elementus->shortName.is_open){
                strcpy(str, get_el_name(elementus));
                write(requester_socket, buffer, strlen(buffer) + 1);
            }
        }
    }

    write(requester_socket, MSG_DONE, sizeof(MSG_DONE));
}

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

void _LIST_GAMERS(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);

    if(check_session(src, requester_socket)){
        src[0] -= '0';

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

void _LEAVE(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);
    char buffer[150] = "Gamers info:";
    char* name = *get_requester_name(game, info);
    strcpy(buffer + sizeof("Gamers info:")-1, name ? name : "Anonimus");
    strcat(buffer, " leave the game");

    if(!info.guest.is_guest){
        gamer_leave(game, info.client.session_num, info.client.id);
        size_t i = game->sessions[info.client.session_num].gamer_count;
        while(i--)
            write(game->sessions[info.client.session_num].gamers[i].data.client_socket, buffer, strlen(buffer)+1);
    }
    else guest_leave(game, info.guest.num);

    if(!src){
        write(requester_socket, MSG_LEAVE, sizeof(MSG_LEAVE));
        close(requester_socket);
    }
}

void _WRONG_REQUEST(game_server* game, char* src, requester_info info){
    int requester_socket = get_requester_socket(game, info);
    write(requester_socket, MSG_WRONG_REQUEST, sizeof(MSG_WRONG_REQUEST));
}