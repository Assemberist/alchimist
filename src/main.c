#include "core/loader.h"
//#include "int_check.h"
#include "service/string_tree.h"

int main(){
	library lib = load_library("/home/assemberist/source/alchimist/alchimist-beta-0.1.0/debug");
	//check_library();
	//draw(&lib);
	for(size_t i = lib.group_count; i--;){
		for(size_t j = lib.groups[i].name_count; j--;)
			puts(get_el_name(lib.groups[i].names + j));
		puts("");
	}

	for(size_t i = lib.recept_count; i--;)
		printf("%s + %s = %s\n", get_el_name(lib.recepts[i].reagent1), get_el_name(lib.recepts[i].reagent2), get_el_name(lib.recepts[i].rezult));

	dispose_library(&lib);
	return 0;
}
