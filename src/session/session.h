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
	char** libraryes;
	size_t library_count;
	session sessions[max_sessions];
	user guests[max_lobby];
	size_t guest_count;
	fd_set lobby;
	int enough;
}game_server;

typedef union requester_info{
    struct{
        int8_t is_guest:1;
        int8_t num:7;
    }guest;
    struct{
        int8_t is_guest:1;
        int8_t session_num:3;
        int8_t id:4;
    }client;
} requester_info;

void new_game(game_server* game);
void open_session(game_server* game, size_t id, char* src);

void new_guest(game_server* game, int sock);
void guest_leave(game_server* game, size_t id);
void new_gamer(game_server* game, requester_info where, requester_info info);
void gamer_leave(game_server* game, size_t session_num, size_t id);

int get_requester_socket(game_server* game, requester_info info);
char** get_requester_name(game_server* game, requester_info info);

#ifdef server_status

void print_server(game_server* game, char* buff);
void print_session(session* party, size_t lvl, char* buff);
void print_gamer(client* g, size_t lvl, char* buff);
void print_guest(user* u, size_t lvl, char* buff);

#define psr(A, B) print_server(A, B)
#define pss(A,B,C) print_session(A,B,C)
#define pgm(A,B,C) print_gamer(A,B,C)
#define pgs(A,B,C) print_guest(A,B,C)

#else

#define psr(A, B)
#define pss(A,B,C)
#define pgm(A,B,C)
#define pgs(A,B,C)

#endif

#endif