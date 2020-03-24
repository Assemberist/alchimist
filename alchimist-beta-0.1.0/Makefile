all: main.o loader.o render.o library.o int_check.o
	gcc -o debug/alchimist obj/loader.o obj/render.o obj/main.o obj/library.o obj/int_check.o obj/string_tree.o -g

main.o: src/main.c
	gcc -c src/main.c -o obj/main.o -g

loader.o: src/loader.c src/loader.h
	gcc -c src/loader.c -o obj/loader.o -g

render.o: src/render.c src/render.h
	gcc -c src/render.c -o obj/render.o -g

library.o: src/library.c src/library.h
	gcc -c src/library.c -o obj/library.o -g

int_check.o: src/int_check.c src/int_check.h
	gcc -c src/int_check.c -o obj/int_check.o -g

string_tree.o: src/string_tree.c src/string_tree.h
	gcc -c src/string_tree.c -o obj/string_tree.o -g
