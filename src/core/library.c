#include "library.h"
#include <stddef.h>
#include <string.h>

element* dualisation(library* lib, element* element1, element* element2, size_t session){
	if(element1 && element2){
		int i;
		for(i = lib->recept_count; i--;){
			combinate* com = lib->recepts + i;
			if((com->reagent1 == element1 && com->reagent2 == element2) || (com->reagent1 == element2 && com->reagent2 == element1)){
				com->rezult->shortName.is_open |= 1 << session;
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

char** search_libs(const char* lib_foulder){
	size_t path_len = sizeof(lib_foulder);

	char buffer[512];
	strcpy(buffer, "find ");
	strcat(buffer, lib_foulder);
	strcat(buffer, "* -maxdepth 0 -type d > ");
	strcat(buffer, lib_foulder);
	strcat(buffer, "library_list.txt");
	system(buffer);

	strcpy(buffer, lib_foulder);
	strcat(buffer, "library_list.txt");

	FILE* file = fopen(buffer, "r");
	if(!file) return 0;

	size_t i = 0;
	while(fgets(buffer, 512, file)) i++;
	
	char** libraries = (char**)malloc(sizeof(char*) * i + 1);
	libraries[i] = NULL;

	rewind(file);

	size_t len;
	while(i--){
		fgets(buffer, 512, file);
		len = strlen(buffer);
		libraries[i] = (char*)malloc(len);
		strcpy(libraries[i], buffer);
		libraries[i][len-1] = '\0';
	}

	fclose(file);
	
	return libraries;
}
