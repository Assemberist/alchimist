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
void* find_element(char* src, token* begin);
void remove_tree(token* begin);

#endif
