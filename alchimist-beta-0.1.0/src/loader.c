#include "loader.h"
#include "string_tree.h"

char* get_lexem(FILE* file, char stop_sym){
	if(feof(file)) return 0;

	char buff[64];
	fgets(buff, 64, file);
	char* ptr = strchr(buff, stop_sym);
	if(*(ptr+1) == '0') buff[0] |= 128;
	*ptr = '\0';

	size_t len = strlen(buff);
	ptr = (char*)malloc(len);
	strncpy(ptr, buff, len);
	return ptr;
}

group parse_group(FILE* file){
	group gr;
	gr.name_count = 0;
	size_t capasity = 0;
	
	char buff[64];
	memset(buff, 0, 64);

	while(fgets(buff, 64, file)) capasity++;
	gr.names = (char**)malloc(capasity * sizeof(char*));

	rewind(file);

	while(gr.name_count < capasity){
		gr.names[gr.name_count] = get_lexem(file, ':');
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
		puts("dir reading error");
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
				puts("group read error");
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
		char* ptr = buff;
		char* end = strchr(ptr, '+');
		*end = '\0';
        lib.recepts[lib.recept_count].reagent1 = find_word(ptr, worterbuch);

		ptr = end + 1;
		end = strchr(ptr, '=');
		*end = '\0';
        lib.recepts[lib.recept_count].reagent2 = find_word(ptr, worterbuch);

		ptr = end + 1;
		end = strchr(ptr, '\n');
		*end = '\0';
        lib.recepts[lib.recept_count].rezult = find_word(ptr, worterbuch);
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
	for(i = lib.group_count; i--;){
		for(j = lib.groups[i].name_count; j--;){
			if(lib.groups[i].names[j][0] & 128){
				lib.groups[i].names[j][0] ^= 128;
				add_word(lib.groups[i].names[j], wortbook);
				lib.groups[i].names[j][0] ^= 128;
			}
			else add_word(lib.groups[i].names[j], wortbook);
		}
	}

	library combinates = load_combinates(wortbook, path);
	lib.recepts = combinates.recepts;
	lib.recept_count = combinates.recept_count;

	remove_tree(wortbook);
	return lib;
}
