#include "library.h"

element* dualisation(library* lib, element* element1, element* element2){
	if(element1 && element2){
		int i;
		for(i = lib->recept_count; i--;){
			combinate* com = lib->recepts + i;
			if((com->reagent1 == element1 && com->reagent2 == element2) || (com->reagent1 == element2 && com->reagent2 == element1)){
				com->rezult->shortName.is_open = 1;
				return com->rezult;
			}
		}
	}
	return 0;
}

void dispose_library(library* lib){
	int i = lib->group_count;
	while(i--){
		int j = lib->groups[i].name_count;
		while(j--){
			if(lib->groups[i].names[j].shortName.is_long)
				free(lib->groups[i].names[j].longName.name);
		}
		free(lib->groups[i].names);
		free(lib->groups[i].name);
	}
	free(lib->groups);
	free(lib->recepts);
	remove_tree(lib->worterbuch);
}

char* get_el_name(element* ptr){
	return (ptr->shortName.is_long ? ptr->longName.name : ptr->shortName.name);
}
