debug: dbg := "-g"
debug: compile assemble

build: dbg := 
build: compile assemble

all: compile build tests

compile:
	gcc -shared -o libalch_core.so src/core/*.c $(dbg)
	#gcc -c src/server/*.c $(dbg)
	gcc -c src/service/*.c $(dbg)
	gcc -c src/main.c $(dbg)
	mv *.o obj/

assemble:
	gcc -L. obj/* -o alchimist -lalch_core $(dbg)

clean:
	rm obj/*

