#include "session.h"
#include <stddef.h>

#include <stdint.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include "protocol.h"

int main(){
	game_server game;
	new_game(&game);
	game.libraryes = search_libs("/home/assemberist/source/alchimist/", &game.library_count);


    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    serv_addr.sin_port = htons(5000);
	
	int listener = socket(AF_INET, SOCK_STREAM | O_NONBLOCK, 0);
    if(bind(listener, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		perror("cannot bind address");
		return -1;
	}
	listen(listener, 10);

	struct timeval ts = {0,10000};

#ifdef server_status
	char debug_buffer[20000];

	psr(&game, debug_buffer);
	puts(debug_buffer);

#endif

	char reader[500];

	while(!game.enough){
		int tmp_sock;
		if((tmp_sock = accept(listener, NULL, NULL)) != -1){
			fcntl(tmp_sock, F_SETFL, O_NONBLOCK);
			printf("One more connection on socket %i\n", tmp_sock);
			new_guest(&game, tmp_sock);
		}

		// check lobby
		switch(select(1024, NULL, &game.lobby, NULL, &ts)){
			case -1:
			case 0:
				break;
			default:{
				size_t i = game.guest_count;
				while(i--){
					int requester = game.guests[i].client_socket;
					if(FD_ISSET(requester, &game.lobby))
						switch(read(requester, reader, 500)){
							case -1:
								break;
							case 0:
								printf("socket %i closed\n", requester);
								guest_leave(&game, i);
								close(requester);
								break;
							default:{
								printf("new request from %i: %s\n", requester, reader);
								requester_info info;
								info.guest.is_guest = 1;
								info.guest.num = i;
								handle_request(&game, reader, info);
							}
						}
				}
			}
		}

		// check started sessions
		size_t j = max_sessions;
		while(j--){
			session* party = game.sessions + j;
			switch(select(1024, NULL, &party->readers, NULL, &ts)){
				case -1:
				case 0:
					break;
				default:{
					size_t i = party->gamer_count;
					while(i--){
						int gomer = party->gamers[i].data.client_socket;
						if(FD_ISSET(gomer, &party->readers))
							switch(read(gomer, reader, 500)){
								case -1:
									break;
								case 0:
									printf("gamer on socket %i closed\n", gomer);
									gamer_leave(&game, j, i);
									close(gomer);
									break;
								default:{
									printf("new request from %i: %s", gomer, reader);
									requester_info info;
									info.client.is_guest = 0;
									info.client.session_num = j;
									info.client.id = i;
									handle_request(&game, reader, info);
								}
							}
					}
				}
			}
		}
	}

	while (game.library_count--)
		free(game.libraryes[game.library_count]);
	free(game.libraryes);

	return 0;
}