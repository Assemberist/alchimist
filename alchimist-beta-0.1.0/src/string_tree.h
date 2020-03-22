#ifndef STRING_TREE
#define STRING_TREE

#include <stdlib.h>
#include <string.h>

typedef struct token{
	char* tok;
	char* origin;
	struct token* next;
	struct token* down; 
}token;

token* init_tree();
void add_word(char*, token*);
void remove_tree(token*);

#endif
