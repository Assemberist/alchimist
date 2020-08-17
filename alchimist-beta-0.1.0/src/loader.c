#include "loader.h"
#include "string_tree.h"

char* get_lexem(FILE* file, char stop_sym){
	char buff[64];
	char* ptr = buff;
	if(feof(file)) return 0;
	do *ptr=fgetc(file);
	while(!feof(file) && *(ptr++) != stop_sym);
	size_t len = ptr-buff-1;
	ptr = (char*)malloc(len+1);
	strncpy(ptr, buff, len);
	ptr[len] = '\0';
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

	while(gr.name_count < capasity)
	{
		gr.names[gr.name_count] = get_lexem(file, ':');
		if(fgetc(file) == '0') gr.names[gr.name_count][0] += 128;
		while(!feof(file))
			if('\n' == fgetc(file)) break;
		gr.name_count++;
	}
	return gr;
}

library load_groups(){
	library lib;
	lib.groups = NULL;
	lib.group_count = 0;

	FILE* reader;

	char path[PATH_MAX];
	memset(path, 0, PATH_MAX);
	if(readlink("/proc/self/exe", path, PATH_MAX) == -1){ 
		puts("exe pos error");
		return lib;
	}
	path[strlen(path)-9] = '\0';

	char group_name[PATH_MAX];
	strcpy(group_name, path);
	strcat(group_name, "groups");

	DIR* dir;
	if((dir = opendir(group_name)) == NULL){
		puts("dir reading error");
		return lib;
	}
	struct dirent* f_cur;

	size_t file_count = 0;
	while((f_cur = readdir(dir)) != NULL) file_count++;
	file_count-=2;

	rewinddir(dir);

	lib.groups = (group*)malloc(sizeof(group) * file_count);

	while(lib.group_count < file_count){
		f_cur = readdir(dir);
		if(f_cur->d_name[0] != '.'){
			strcpy(group_name, path);
			strcat(group_name, "groups/");
			strcat(group_name, f_cur->d_name);
			group g;
			reader = fopen(group_name, "r");
			if(!reader){
				puts("group read error");
				return lib;
			}
			g = parse_group(reader);
			g.name = (char*)malloc(strlen(f_cur->d_name) - 4);
			strncpy(g.name, f_cur->d_name, strlen(f_cur->d_name) - 4);
			lib.groups[lib.group_count] = g;
			lib.group_count++;
		}
	}
	fclose(reader);
	closedir(dir);
	
	return lib;
}

library load_combinates(token* worterbuch){
	library lib;
    lib.recepts = NULL;

	FILE* reader;
	char path[PATH_MAX];
	memset(path, 0, PATH_MAX);
	if(readlink("/proc/self/exe", path, PATH_MAX) == -1){ 
		puts("exe pos error");
		return lib;
	}
	path[strlen(path)-9] = '\0';

	char group_name[PATH_MAX];
	strcpy(group_name, path);
	strcat(group_name, "combinates.txt");
	reader = fopen(group_name, "r");

	char buff[200];

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

library load_library(){
	library lib;

	library groups = load_groups();
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

	library combinates = load_combinates(wortbook);
	lib.recepts = combinates.recepts;
	lib.recept_count = combinates.recept_count;
	
	remove_tree(wortbook);
	return lib;
}
