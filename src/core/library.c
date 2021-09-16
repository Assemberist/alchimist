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

void parse_lib(char* src, void* el, void*){
	size_t len = strlen(src);
	*((char**)el) = malloc(len);
	strcpy(*(char**)el, src);
	(*((char**)el))[len-1] = '\0';
}

char** search_libs(const char* lib_foulder, size_t* len){
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

	return read_file(buffer, parse_lib, sizeof(char*), NULL, len);
}

void* read_file(char* path, void(*parser)(char*, void*, void*), size_t element_size, void* args, size_t* rows){
	FILE* file = fopen(path, "r");
	if(!file) return 0;

	char buffer[512];

	size_t i = 0;
	while(fgets(buffer, 512, file)) i++;
	if(rows) *rows = i;
	
	size_t len = element_size * i;
	void* ptr = malloc(len);

	rewind(file);

	for(i = 0; i<len; i+=element_size){
		fgets(buffer, 512, file);
		parser(buffer, (int8_t*)ptr + i, args);
	}

	fclose(file);
	return ptr;
}
