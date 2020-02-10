#include "library.h"

char* dualisation(library* lib, char* element1, char* element2){
	int i;
	for(i = lib->recept_count; i--;){
		combinate* com = lib->recepts + i;
		if((com->reagent1 == element1 && com->reagent2 == element2) || (com->reagent1 == element2 && com->reagent2 == element1)){
			char* ptr = com->rezult;
			if(ptr[0] >> 7) ptr[0] ^= 1<<7;
			return ptr;
		}
	}
	return 0;
}

void dispose_library(library* lib){
	int i = lib->group_count;
	while(i--){
		int j = lib->groups[i].name_count;
		while(j--) free(lib->groups[i].names[j]);
		free(lib->groups[i].names);
		free(lib->groups[i].name);
	}
	free(lib->groups);
	free(lib->recepts);
}
