SRCs=$(wildcard *.c)

test: wb.o core.o test_archive.alch2	
	gcc wb.o core.o -o test

core.o: $(SRCs:.c=.o)
	ld -r $+ -o $@

%.o: %.c %.h
	gcc -c $< -o $@



wb.o: string_tree/wb.o
	cp $< $@

string_tree/wb.o: $(wildcard string_tree/*.c) $(wildcard string_tree/*.h)
	cd string_tree && make



test_archive.alch2: ../archivator/library.alch2
	cp $< $@

../archivator/library.alch2: ../archivator/parser
	cd ../archivator && ./parser for_test

../archivator/parser:
	cd ../archivator && make