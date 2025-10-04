build:
	./build.sh

clean:
	./clean.sh

rebuild: clean build

run:
	./alchimist library.alch2
