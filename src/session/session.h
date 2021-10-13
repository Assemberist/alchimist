#ifndef SESSION_H
#define SESSION_H

#include "../core/loader.h"

#define gamer_capacity 16
#define max_sessions 7
#define max_lobby 100

typedef struct user{
	int client_socket;
	char* login;
}user;

typedef struct client{
	user data;
	int16_t discover;
	int16_t attempts;
}client;

typedef struct session{
	library lib;
	client gamers[gamer_capacity];
	fd_set readers;
	char* path;
	int16_t gamer_count;
	int16_t session_id;
	int16_t max_desc;
}session;

typedef struct game_server{
	session sessions[max_sessions];
	user guests[max_lobby];
	size_t guest_count;
	fd_set lobby;
}game_server;

void new_game(game_server* game);

void new_guest(game_server* game, int sock);
void guest_leave(game_server* game, size_t id);
void gamer_leave(game_server* game, size_t session_num, size_t id);

#endif