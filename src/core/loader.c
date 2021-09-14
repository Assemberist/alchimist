#include "../core/loader.h"

element create_element(char* src){
	element el;
	strtok(src, ":");
	el.shortName.is_long = strlen(src) >= sizeof(char*) * 2 - 1;
	if(el.shortName.is_long){
		el.longName.name = (char*)malloc(strlen(src)+1);
		strcpy(el.longName.name, src);
	}
	else strcpy(el.shortName.name, src);
	el.shortName.is_open = (*strtok(NULL, "") == '0' ? 0 : 127);
	
	return el;
}

group parse_group(FILE* file){
	group gr;
	gr.name_count = 0;
	size_t capasity = 0;

	char buff[64];
	memset(buff, 0, 64);

	while(fgets(buff, 64, file)) capasity++;
	gr.names = (element*)malloc(capasity * sizeof(element));
	memset(gr.names, 0, capasity * sizeof(element));

	rewind(file);

	while(gr.name_count < capasity){
		fgets(buff, 64, file);
		gr.names[gr.name_count] = create_element(buff);
		gr.name_count++;
	}
	return gr;
}

library load_groups(char* path){
	library lib;
    lib.groups = NULL;
	lib.group_count = 0;

	FILE* reader;

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
			reader = fopen(group_name, "r");
			if(!reader){
				perror("group read error");
				return lib;
			}
			g = parse_group(reader);
			g.name = (char*)malloc(strlen(file_name) - 4);
			strncpy(g.name, file_name, strlen(file_name) - 4);
			lib.groups[lib.group_count] = g;
			lib.group_count++;
		}
	}
	fclose(reader);
	closedir(dir);

	return lib;
}

library load_combinates(token* worterbuch, char* path){
	library lib;
    lib.recepts = NULL;

	char group_name[PATH_MAX];
	sprintf(group_name, "%s%s", path, "/combinates.txt");

	char buff[200];

	FILE* reader = fopen(group_name, "r");
	if(!reader){
		puts("combinates.txt not open");
		return lib;
	}

	size_t str_count = 0;
	while(!feof(reader)) {
		fgets(buff, 200, reader);
		str_count++;
	}
	rewind(reader);

	lib.recepts = (combinate*)malloc(sizeof(combinate) * str_count);

	for(lib.recept_count = 0; lib.recept_count < str_count - 1; lib.recept_count++){
		fgets(buff, 200, reader);
		char* ptr = strtok(buff, "+");
        lib.recepts[lib.recept_count].reagent1 = find_element(ptr, worterbuch);

		ptr = strtok(NULL, "=");
        lib.recepts[lib.recept_count].reagent2 = find_element(ptr, worterbuch);

		ptr = strtok(NULL, "\n");
        lib.recepts[lib.recept_count].rezult = find_element(ptr, worterbuch);
	}
	fclose(reader);
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
