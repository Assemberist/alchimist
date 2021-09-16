#include "session.h"
#include <stddef.h>

int main(){
	// create socket to listen new connections
	size_t len;
	char** list = search_libs("/home/assemberist/source/alchimist/", &len);
	while (len--) {
		puts(list[len]);
		free(list[len]);
	}

	free(list);

	game_server game;
	game.open_sessions = 0;
	
	//while(1){
		// check new connections
		// if connected
		
		// check messages from exsisting clients
	//}

	return 0;
}