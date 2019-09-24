#include "loader.h"

char* find_lexem(library* lib, char* buff){
	size_t i, j;
	for(i = lib->group_count; i--;)
		for(j = lib->groups[i].name_count; j--;){
			if(strstr(lib->groups[i].names[j], buff)) return lib->groups[i].names[j];
			buff[0] ^= 1<<7;
			if(strstr(lib->groups[i].names[j], buff)) return lib->groups[i].names[j];
			buff[0] ^= 1<<7;
		}
	return 0;
}

void read_lexem(FILE* file, char* buff, size_t buff_size, char stop_sym){
	char sym;
	while(buff_size--){
		sym = fgetc(file);
		if(sym == stop_sym || feof(file)) return;
		*(buff++) = sym;
	}
	puts("buff end");
}

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

library load_library(){
	library lib;
	lib.groups = NULL;
    lib.recepts = NULL;
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
			strncat(g.name, f_cur->d_name, strlen(f_cur->d_name) - 4);
			lib.groups[lib.group_count] = g;
			lib.group_count++;
		}
	}
	fclose(reader);
	closedir(dir);

	strcpy(group_name, path);
	strcat(group_name, "combinates.txt");

	reader = fopen(group_name, "r");
	char buff[64];

	if(!reader){
		puts("combinates.txt not open");
		return lib;
	}
	size_t str_count = 0;
	while(!feof(reader)) if(fgetc(reader) == '\n') str_count++;
	rewind(reader);

	lib.recepts = (combinate*)malloc(sizeof(combinate) * str_count);

	for(lib.recept_count = 0; lib.recept_count < str_count; lib.recept_count++){
		memset(buff, 0, 64);
		read_lexem(reader, buff, 64, '+');
        lib.recepts[lib.recept_count].reagent1 = find_lexem(&lib, buff);
		memset(buff, 0, 64);
		read_lexem(reader, buff, 64, '=');
        lib.recepts[lib.recept_count].reagent2 = find_lexem(&lib, buff);
		memset(buff, 0, 64);
		read_lexem(reader, buff, 64, '\n');
        lib.recepts[lib.recept_count].rezult = find_lexem(&lib, buff);
	}
	fclose(reader);
	return lib;
}
