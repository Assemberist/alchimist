#include "session.h"

session new_session(char* path, int16_t id){
	return (session){load_library(path), NULL, path, 0, id};
}

int gamer_connect(session* ses, char* login, int new_socket){
	if(ses->gamer_count == gamer_capacity)
		return -1;
	
	ses->gamers[ses->gamer_count] = (client){new_socket, login, 0, 0};
	return 0;
}

void gamer_disconnect(session* ses, size_t client_num){
	if(ses->gamer_count)
		ses->gamers[client_num] = ses->gamers[--ses->gamer_count];
}