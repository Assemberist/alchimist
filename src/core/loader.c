#include "../core/loader.h"
#include <stddef.h>

void create_element(char* src, void* el, void* args){
	strtok(src, ":");
	((element*)el)->shortName.is_long = strlen(src) >= sizeof(char*) * 2 - 1;
	if(((element*)el)->shortName.is_long){
		((element*)el)->longName.name = (char*)malloc(strlen(src)+1);
		strcpy(((element*)el)->longName.name, src);
	}
	else strcpy(((element*)el)->shortName.name, src);
	((element*)el)->shortName.is_open = (*strtok(NULL, "\n") == '0' ? 0 : 127);
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
		if(file_name[0] != '.'){
            sprintf(group_name, "%s%s%s", path, "/groups/", file_name);

			group g;
			g.names = read_file(group_name, create_element, sizeof(element), NULL, &g.name_count);
			g.name = (char*)malloc(strlen(file_name) - 4);
			strncpy(g.name, file_name, strlen(file_name) - 4);

			lib.groups[lib.group_count] = g;
			lib.group_count++;
		}
	}
	closedir(dir);

	return lib;
}

void create_combinate(char* src, void* el, void* args){
	char* ptr = strtok(src, "+");
	((combinate*)el)->reagent1 = find_element(ptr, (token*)args);

	ptr = strtok(NULL, "=");
	((combinate*)el)->reagent2 = find_element(ptr, (token*)args);

	ptr = strtok(NULL, "\n");
	((combinate*)el)->rezult = find_element(ptr, (token*)args);
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

	library groups = load_groups(path);
	lib.groups = groups.groups;
	lib.group_count = groups.group_count;

	token* wortbook = init_tree();
	int i, j;
	for(i = lib.group_count; i--;)
		for(j = lib.groups[i].name_count; j--;)
			add_element(get_el_name(lib.groups[i].names + j), lib.groups[i].names + j, wortbook);

	library combinates = load_combinates(wortbook, path);
	lib.recepts = combinates.recepts;
	lib.recept_count = combinates.recept_count;

	lib.worterbuch = wortbook;
	return lib;
}
