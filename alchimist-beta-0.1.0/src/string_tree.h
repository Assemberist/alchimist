#ifndef STRING_TREE
#define STRING_TREE

#include <stdlib.h>
#include <string.h>

typedef struct token{
	char* tok;
	void* origin;
	struct token* next;
	struct token* down; 
}token;

token* init_tree();
void add_element(char* src, void* element, token* begin);
void add_word(char* src, token* begin);		// Saved for bc.
void* find_element(char* src, token* begin);
char* find_word(char* src, token* begin);	// Saved for bc.
void remove_tree(token* begin);

#endif
