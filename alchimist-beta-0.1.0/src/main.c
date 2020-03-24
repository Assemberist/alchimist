#include "loader.h"
#include "render.h"
#include "int_check.h"
#include "string_tree.h"

int main(){
	token* wortbook = init_tree();
	library lib = load_library();

	size_t i = lib.group_count;
	while(i--){
		size_t j = lib.groups[i].name_count;
		while(j--){
			char* src = lib.groups[i].names[j];
			if(src){
				char val = *src & 127;
				*src &= 127;
				add_word(src, wortbook);
				*src |= val;
			}
		}
	}

	return 0;
}
