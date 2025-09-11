alchimist: wb.o $(wildcard *.c) $(wildcard *.h)
	gcc $+ -o $@


wb.o: string_tree/wb.o
	cp $< $@

string_tree/wb.o: $(wildcard string_tree/*.c) $(wildcard string_tree/*.h)
	cd string_tree && make