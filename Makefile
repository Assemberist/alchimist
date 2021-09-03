dbg = "-g"

debug: compile libcore_d
	gcc -L. obj/* -o alchimist -lalch_core -g

build: compile libcore_b
	gcc -L. obj/* -o alchimist -lalch_core

all: compile build tests

compile:
	#gcc -c server/*.c $(dbg)
	gcc -c src/service/*.c $(dbg)
	gcc -c src/main.c $(dbg)
	mv *.o obj/

libcore_b:
	cd src/core; make build

libcore_d:
	cd src/core; make debug

clean:
	rm obj/*

