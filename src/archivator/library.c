#include "library.h"

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

void create_element(char* src, void* el, void* args){
    element* elem = (element*)el;

    if(src[0] == ':'){
        elem->is_open = true;
        src++;
    }
    else elem->is_open = false;

	strtok(src, "\n");
	elem->name = (char*)malloc(strlen(src)+1);
	strcpy(elem->name, src);
}

library load_groups(char* path){
	library lib;
    lib.groups = NULL;
	lib.group_count = 0;

	char group_name[PATH_MAX];
    sprintf(group_name, "%s%s", path, "/groups");

	DIR* dir;
	if((dir = opendir(group_name)) == NULL){
		perror("dir reading error");
		return lib;
	}

	size_t file_count = 0;
	struct dirent* f_cur;
	while(f_cur = readdir(dir)) file_count++;
	file_count-=2;

	rewinddir(dir);

	lib.groups = (group*)malloc(sizeof(group) * file_count);

	while(lib.group_count < file_count){
		f_cur = readdir(dir);
        char* file_name = f_cur->d_name;
        size_t len = strlen(file_name);
		if(file_name[0] != '.'){
            sprintf(group_name, "%s%s%s", path, "/groups/", file_name);

			group g;
			g.names = read_file(group_name, create_element, sizeof(element), NULL, &g.name_count);
			g.name = (char*)malloc(len - 3);
			strncpy(g.name, file_name, len - 4);
			g.name[len-4] = '\0';

			lib.groups[lib.group_count] = g;
			lib.group_count++;
		}
	}
	closedir(dir);

	return lib;
}

void create_combinate(char* src, void* el, void* args){
	char* ptr = strtok(src, "+");
	((combinate*)el)->reagent1 = get_value(ptr, (token*)args);

	ptr = strtok(NULL, "=");
	((combinate*)el)->reagent2 = get_value(ptr, (token*)args);

	ptr = strtok(NULL, "\n");
	((combinate*)el)->rezult = get_value(ptr, (token*)args);
}

library load_combinates(token* worterbuch, char* path){
	library lib;

	char group_name[PATH_MAX];
	sprintf(group_name, "%s%s", path, "/combinates.txt");

	lib.recepts = read_file(group_name, create_combinate, sizeof(combinate), worterbuch, &lib.recept_count);
	return lib;
}

library load_library(char* path){
	library lib;
	token* wb;

	library groups = load_groups(path);
	lib.groups = groups.groups;
	lib.group_count = groups.group_count;

	wb = wordbook_from_library(&lib);

	library combinates = load_combinates(wb, path);
	lib.recepts = combinates.recepts;
	lib.recept_count = combinates.recept_count;

	remove_tree(wb);
	return lib;
}

void dispose_library(library* lib){
	int i = lib->group_count;
	while(i--){
		int j = lib->groups[i].name_count;
		while(j--){
            free(lib->groups[i].names[j].name);
		}
		free(lib->groups[i].names);
		free(lib->groups[i].name);
	}
	free(lib->groups);
	free(lib->recepts);
}

void parse_lib(char* src, void* el, void* _arg){
	size_t len = strlen(src);
	*((char**)el) = malloc(len);
	strcpy(*(char**)el, src);
	(*((char**)el))[len-1] = '\0';
}

token* wordbook_from_library(library* lib){
	token* wortbook = init_tree();
	int i, j;
	for(i = lib->group_count; i--;)
		for(j = lib->groups[i].name_count; j--;)
			set_value(lib->groups[i].names[j].name, lib->groups[i].names + j, wortbook);

	return wortbook;
}
