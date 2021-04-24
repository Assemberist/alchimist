#include "loader.h"
#include "render.h"
//#include "int_check.h"
#include "string_tree.h"

int main(){
	library lib = load_library("./debug");
	//check_library();
	//draw(&lib);
	for(size_t i = lib.group_count; i--;){
		for(size_t j = lib.groups[i].name_count; j--;)
			puts(lib.groups[i].names[j].value);
		puts("");
	}

	for(size_t i = lib.recept_count; i--;)
		printf("%s + %s = %s\n", lib.recepts[i].reagent1->value, lib.recepts[i].reagent2->value, lib.recepts[i].rezult->value);

	dispose_library(&lib);
	return 0;
}
