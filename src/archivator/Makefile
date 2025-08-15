parser.exe: syntax.tab.c \
			lex.yy.c \
			$(wilcard *.h) \
			$(wilcard *.c)
	g++ *.c -o parser.exe

syntax.tab.c: syntax.y
	win_bison.exe -d syntax.y

lex.yy.c: lexic.lex
	win_flex.exe lexic.lex

clean:
	del parser.exe
	del syntax.tab.c
	del syntax.tab.h
	del lex.yy.c
