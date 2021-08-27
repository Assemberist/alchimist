#include "library.h"

element* dualisation(library* lib, element* element1, element* element2){
	int i;
	for(i = lib->recept_count; i--;){
		combinate* com = lib->recepts + i;
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

element create_element(char* src){
	element el;

	char* ptr = strtok(buff, ":");

	if(el.shortName.is_long = strlen(src) >= sizeof(char*) * 2 - 1)
		strcpy(el.shortName.name, buff);
	else {
		el.longName.name = (char*)malloc(strlen(buff)+1);
		strcpy(el.longName.name, buff);
	}
	el.shortName.is_open = (*strtok(NULL, "") == '0' ? 0 : 1);
	
	return el;
}

char* get_el_name(element* ptr){
	return (ptr->shortName.is_long ? ptr->longName.name : ptr->shortName.name);
}