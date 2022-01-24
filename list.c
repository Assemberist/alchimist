#include "src/core/loader.h"

int main(){
	library lib = load_library("./debug");
	for(size_t i = lib.group_count; i--;){
		for(size_t j = lib.groups[i].name_count; j--;){
			char* src = get_el_name(lib.groups[i].names + j);
			printf("%s\t0x%i\n", src, src);
		}
		puts("");
	}

	for(size_t i = lib.recept_count; i--;)
		printf("%s + %s = %s\n", get_el_name(lib.recepts[i].reagent1), get_el_name(lib.recepts[i].reagent2), get_el_name(lib.recepts[i].rezult));

	dispose_library(&lib);
	return 0;
}
