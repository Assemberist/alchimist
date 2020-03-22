#include "string_tree.h"

void remove_rec(token* begin){
	if(begin->down) {
		remove_rec(begin->down);
		free(begin->down);
	}
	if(begin->next){
		remove_rec(begin->next);
		free(begin->next);
	}
}

void remove_tree(token* begin){
	size_t len = 128;
	while(len--) remove_rec(begin + len);
}

token* init_tree(){
	token* ptr = (token*)malloc(sizeof(token)<<7);
	memset(ptr, 0, sizeof(token)<<7);
	return ptr;
}

token* new_token(char* _token){
	token* ret = (token*)malloc(sizeof(token));
	memset(ret, 0, sizeof(token));
	ret->tok = (char*)malloc(strlen(_token)+1);
	strcpy(ret->tok, _token);
	return ret;
}

size_t strdif(char* str1, char* str2){
	size_t ret = 0;
	while(*(str1++)==*(str2++)) ++ret;
	return ret;
}

token* slide_down(char* src, token* begin){
	char syn;
	goto enter;

	do{
		begin = begin->next;
	
		enter:
	
		if(syn = strdif(begin->tok, src)){
			if(syn < strlen(begin->tok)){
				token* ptr = new_token(begin->tok + syn);
				ptr->origin = begin->origin;
				begin->origin = 0;
				ptr->down = begin->down;
				begin->tok[syn] = 0;
				begin->down = ptr;
			}
			if(syn < strlen(src)) 
				return begin->down ? slide_down(src + syn, begin->down) : (begin->down = new_token(src + syn));
			return begin;
		}
	}while(begin->next);

	begin->next = new_token(src);
	return begin->next;
}

void add_word(char* src, token* begin){
	token* ptr = begin + *src;
	if(ptr->tok) slide_down(src, ptr)->origin = src;
	else{
		ptr->origin = src;
		ptr->tok = (char*)malloc(strlen(src+1));
		strcpy(ptr->tok, src);
	}
}
