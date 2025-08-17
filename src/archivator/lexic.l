%{

#include "syntax.tab.h"
#include "parser.h"

extern phase Phase;
extern token* wordbook;

static size_t ids = 0;

%}

%option noyywrap

%%

\[[Oo]pen\]     	return OPEN;
[\n\t ]+			return SPACE;

\+					return PLUS;
=					return EQ;

<<EOF>>				return END;

[:alnum:]+			goto lbl_word;

\{[:alnum: ]+\}		{
						yytext++;
						yytext[strlen(yytext)-1] = '\0';

					lbl_word:
						if(Phase == PHASE3) {
							yylval.id = (size_t)get_value(yytext, wordbook);
							return ID;
						}
						else {
							yylval.str = yytext;
							return WORD_P1;
						}
					}

[:alnum: ]+\0		{ set_value(yytext, (void*)ids++, wordbook); return WORD_P2; }

%%
