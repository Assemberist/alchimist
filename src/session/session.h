#ifndef SESSION_H
#define SESSION_H

#include <stdint.h>
#include "../core/loader.h"

#define gamer_capacity 20
#define max_sessions 7

typedef struct client{
	int client_socket;
	char* login;
	int16_t discover;
	int16_t attempts;
}client;

typedef struct session{
	library lib;
	client gamers[gamer_capacity];
	char* path;
	int16_t gamer_count;
	int16_t session_id;
}session;

typedef struct game_server{
	session sessions[max_sessions];
	int listener;
	size_t open_sessions;
}game_server;

#endif