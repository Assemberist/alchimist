dbg = "-g"

debug: compile	
	gcc ../obj/* -o ../debug/alchimist -g

build: compile
	gcc ../obj/* -o alchimist

all: compile build tests

compile:
	gcc -c core/*.c $(dbg)
	#gcc -c server/*.c $(dbg)
	gcc -c service/*.c $(dbg)
	gcc -c main.c $(dbg)
	mv *.o ../obj/

clean:
	rm ../obj/*

