%{
#include "syntax.tab.h"
#include "string_tree.h"

extern phase Phase;
extern token* wordbook;

static size_t ids = 0;

%}

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
							yylval.id = get_value(wordbook, yytext);
							return ID;
						}
						else {
							yylval.str = yytext;
							return WORD_P1;
						}
					}

[:alnum: ]+\0		{ set_value(wordbook, yytext, ids++); return WORD_P2; }

%%