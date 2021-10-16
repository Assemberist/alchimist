#include "session.h"

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
    }
}

void guest_leave(game_server* game, size_t id){
    FD_CLR(game->guests[id].client_socket, &game->lobby);
    free(game->guests[id].login);
    game->guests[id] = game->guests[game->guest_count];
    game->guest_count--;
}

void new_gamer(game_server* game, size_t session_num, size_t id, requester_info info){
    // name should be copyed with mem allocation
}

void gamer_leave(game_server* game, size_t session_num, size_t id){
	
}

void open_session(game_server* game, size_t id, char* src){
    
}
