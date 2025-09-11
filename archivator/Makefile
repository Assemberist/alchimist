parser: $(wildcard *.h) \
		$(wildcard *.c) \
		syntax.tab.c \
		lex.yy.c \
		wb.o
	gcc *.c wb.o -o parser -g



wb.o: string_tree/wb.o
	cp $< $@

string_tree/wb.o: $(wildcard string_tree/*.c) $(wildcard string_tree/*.h)
	cd string_tree && make



syntax.tab.c: syntax.y
	bison -d $@

lex.yy.c: lexic.l
	flex $@



clean:
	cd string_tree && make clean
	rm 	parser \
		syntax.tab.c \
		syntax.tab.h \
		lex.yy.c \
		wb.o
	rm 	binary.dat \
		elements.txt \
		groups.txt \
		library.alch2
