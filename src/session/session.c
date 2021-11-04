#include "session.h"
#include <stdio.h>
#include <string.h>

void new_game(game_server* game){
    game->guest_count = 0;
    FD_ZERO(&game->lobby);
    size_t i = max_sessions;
    while(i--){
        game->sessions[i].gamer_count = 0;
        game->sessions[i].max_desc = 0;
        game->sessions[i].path = NULL;
        game->sessions[i].session_id = i;
        FD_ZERO(&game->sessions[i].readers);
        game->enough = 0;
    }
}

void new_guest(game_server* game, int sock){
    if(game->guest_count < max_lobby){
        game->guests[game->guest_count].client_socket = sock;
        game->guests[game->guest_count].login = NULL;
        game->guest_count++;
        FD_SET(sock, &game->lobby);

        #ifdef request_trace
            printf("Set num %i for guest", game->guest_count-1);
        #endif
    }
}

void guest_leave(game_server* game, size_t id){
    FD_CLR(game->guests[id].client_socket, &game->lobby);
    free(game->guests[id].login);
    game->guests[id] = game->guests[game->guest_count];
    game->guest_count--;
}

void new_gamer(game_server* game, requester_info where, requester_info info){
    session* ptr = game->sessions + info.client.session_num;
    client* cli = ptr->gamers + info.client.id;

    char** first_name = get_requester_name(game, where);
    cli->data.login = *first_name;
    *first_name = NULL;

    size_t client_sock = get_requester_socket(game, where);
    cli->data.client_socket = client_sock;
    if(client_sock > ptr->max_desc) ptr->max_desc = client_sock+1;
    FD_SET(client_sock, &ptr->readers);
    ptr->gamer_count++;

    cli->attempts = 0;
    cli->discover = 0;
}

void gamer_leave(game_server* game, size_t session_num, size_t id){
    session* ptr = game->sessions + session_num;
    client* cli = ptr->gamers + id;
    free(cli->data.login);
    FD_CLR(cli->data.client_socket, &ptr->readers);
    *cli = *(ptr->gamers + ptr->gamer_count - 1);
    ptr->gamer_count--;
}

void open_session(game_server* game, size_t id, char* src){
    game->sessions[id].lib = load_library(src);
    game->sessions[id].path = src;
    FD_ZERO(&game->sessions[id].readers);
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

#ifdef server_status

void print_server(game_server* game, char* buff){
    strcpy(buff, "server state {\n\tlibs {");

    size_t i;
    for(i = 0; i < game->library_count; i++){
        strcat(buff, "\n\t\t");
        strcat(buff, game->libraryes[i]);
    }
    strcat(buff, "\n\t}\n\n");

    for(i = 0; i<max_sessions; i++)
        pss(game->sessions + i, 2, buff);

    strcat(buff, "\t}\n\n");

    for(i = 0; i<game->guest_count; i++)
        pgs(game->guests + i, 2, buff);

    strcat(buff, "\t}\n}");
}

void print_session(session* party, size_t lvl, char* buff){
    char prefix[10];
    size_t i = lvl;
    for(prefix[i] = '\0'; i--; prefix[i] = '\t');
    buff += strlen(buff);
    sprintf(buff, "%ssession %i {\n%s\tlibrary = %s\n\t%sgamers {\n",
        prefix, party->session_id, prefix, party->path, prefix);

    for(i=0; i<party->gamer_count; i++) 
        pgm(party->gamers + i, lvl+2, buff);

    strcat(buff, prefix);
    strcat(buff, "\t}\n");
    strcat(buff, prefix);
    strcat(buff, "}\n");
}

void print_gamer(client* g, size_t lvl, char* buff){
    char prefix[10];
    for(prefix[lvl] = '\0'; lvl--; prefix[lvl] = '\t');
    buff += strlen(buff);
    sprintf(buff, "%sclient { %s : %i  %i/%i }\n", prefix, g->data.login ? g->data.login : "Anonimus", g->data.client_socket, g->discover, g->attempts);
}

void print_guest(user* u, size_t lvl, char* buff){
    char prefix[10];
    for(prefix[lvl] = '\0'; lvl--; prefix[lvl] = '\t');
    buff += strlen(buff);
    sprintf(buff, "%sguest { %s : %i }\n", prefix, u->login ? u->login : "Anonimus", u->client_socket);
}

#endif