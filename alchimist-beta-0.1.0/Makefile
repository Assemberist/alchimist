all: main.o loader.o render.o library.o
	gcc -o debug/alchimist obj/loader.o obj/render.o obj/main.o obj/library.o -g

main.o: src/main.c
	gcc -c src/main.c -o obj/main.o -g

loader.o: src/loader.c src/loader.h
	gcc -c src/loader.c -o obj/loader.o -g

render.o: src/render.c src/render.h
	gcc -c src/render.c -o obj/render.o -g

library.o: src/library.c src/library.h
	gcc -c src/library.c -o obj/library.o
