#include "library.h"
#include <stddef.h>
#include <string.h>

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

void parse_lib(char* src, void* el, void* _arg){
	size_t len = strlen(src);
	*((char**)el) = malloc(len);
	strcpy(*(char**)el, src);
	(*((char**)el))[len-1] = '\0';
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
