CORE = library.o loader.o string_tree.o

debug: dbg := "-g"
debug: corelib server tests list client

build: dbg := 
build: corelib server tests list

corelib:
	gcc -c src/core/*.c $(dbg) -fPIC
	gcc -c src/service/*.c $(dbg)
	gcc -shared -o libalch_core.so $(CORE) $(dbg)
	mv *.o obj/

server:
	gcc -L. src/session/*.c -o game_server -lalch_core $(dbg)

tests:
	gcc -L. src/testTool/*.c -o tester -lalch_core $(dbg)

list: list.c
	gcc -L. list.c -o list -lalch_core $(dbg)
	
client: src/erl_client/erl_client.erl
	erlc src/erl_client/erl_client.erl