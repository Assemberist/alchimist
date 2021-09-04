#include "int_check.h"

void every_element(library* lib, void(*fun)(element_ext*)){
    size_t i, j;
    for(i = lib->group_count; i--;)
		for(j = lib->groups[i].name_count; j--;)
            fun((element_ext*)(lib->groups[i].names + j));
}

int (*tests[])(library*) = {
	missing, undiscovered, unused, unget,
	again_elements, again_combinate, NULL
};

void init(element_ext* el){ if(el) el->shortName.initial_value = el->shortName.is_open; }
void reset(element_ext* el){ if(el) el->shortName.is_open = el->shortName.initial_value; }

int main(int argc, char** argv){
	if(argc != 2){
		puts("Incorrect amount of args");
		return -1;
	}

	library lib = load_library(argv[1]);
	if(!(lib.recepts && lib.groups))
		return -1;

	every_element(&lib, init);

	int (**test_ptr)(library*);
	for(test_ptr = tests; *test_ptr; test_ptr++){
		if((*test_ptr)(&lib)){
			puts("\nLibrary is inconsistent");
			return -1;
		}

		every_element(&lib, reset);
	}

	puts("\nAll tests completed");
	return 0;
}
