parser: syntax.tab.c \
			lex.yy.c \
			$(wildcard *.h) \
			$(wildcard *.c) \
			string_tree.o
	gcc *.c string_tree.o -o parser -g

string_tree.o: string_tree/string_tree.c string_tree/string_tree.h
	gcc -c string_tree/string_tree.c -o $@

syntax.tab.c: syntax.y
	bison -d syntax.y

lex.yy.c: lexic.l
	flex lexic.l

clean:
	rm 	parser \
		syntax.tab.c \
		syntax.tab.h \
		lex.yy.c
