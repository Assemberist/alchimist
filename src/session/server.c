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
	size_t len;
	char** list = search_libs("/home/assemberist/source/alchimist/", &len);
	
	game_server game;

    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    serv_addr.sin_port = htons(5000);
	
	int listener = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(listener, F_SETFL, O_NONBLOCK);
    if(bind(listener, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		perror("cannot bind address");
		return -1;
	}
	listen(listener, 10);

	struct timeval ts = {0,10000};

	char reader[500];

	while(1){
		int tmp_sock;
		if((tmp_sock = accept(listener, NULL, NULL)) != -1){
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
					if(FD_ISSET(game.guests[i].client_socket, &game.lobby))
						switch(read(game.guests[i].client_socket, reader, 500)){
							case -1:
								break;
							case 0:
								printf("socket %i closed\n", game.guests[i].client_socket);
								guest_leave(&game, i);
								break;
							default:{
								requester_info info;
								info.guest.is_guest = 1;
								info.guest.num = i;
								free(handle_request(&game, reader, info));
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
									break;
								default:{
									requester_info info;
									info.client.is_guest = 0;
									info.client.session_num = j;
									info.client.id = i;
									free(handle_request(&game, reader, info));
								}
							}
					}
				}
			}
		}
	}

	while (len--) {
		puts(list[len]);
		free(list[len]);
	}

	free(list);

	return 0;
}