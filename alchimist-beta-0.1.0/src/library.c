#include "library.h"

element* dualisation(library* lib, element* element1, element* element2){
	int i;
	for(i = lib->recept_count; i--;){
		combinate* com = lib->recepts + i;
		printf("ch1: %d ch2: %d el1: %d el2: %d\n", com->reagent1, com->reagent2, element1, element2);
		if((com->reagent1 == element1 && com->reagent2 == element2) || (com->reagent1 == element2 && com->reagent2 == element1)){
			com->rezult->is_open = 1;
			return com->rezult;
		}
	}
	return 0;
}

void dispose_library(library* lib){
	int i = lib->group_count;
	while(i--){
		int j = lib->groups[i].name_count;
		while(j--) free(lib->groups[i].names[j].value);
		free(lib->groups[i].names);
		free(lib->groups[i].name);
	}
	free(lib->groups);
	free(lib->recepts);
}
