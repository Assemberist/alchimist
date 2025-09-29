%{

#include "syntax.tab.h"
#include "parser.h"

extern phase Phase;
extern token* wordbook;
extern pack package;

static size_t ids = 0;

%}

%option noyywrap

%%

\[[Oo]pen\]     	return OPEN;
[\n\t ]+			return SPACE;

\+					return PLUS;
=					return EQ;

<<EOF>>				return END;

[[:alnum:]]+		goto lbl_word;


\{[[:alnum:] ]+\}	{
						yytext++;
						yytext[strlen(yytext)-1] = '\0';

					lbl_word:
						if(Phase == PHASE3) {
							yylval.id = (size_t)find_pack_element_default(yytext, package);
							return ID;
						}
						else {
							yylval.str = yytext;
							return WORD_P1;
						}
					}

[[:alnum:] ]+\0		{
						if(ids) set_value(yytext, (void*)ids++, wordbook); 
						else { set_value(yytext, (void*)UINT64_MAX, wordbook); ids++; } // WA for NULL value
						return WORD_P2;
					}

%%
