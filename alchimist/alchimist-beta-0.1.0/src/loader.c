#include "loader.h"

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
	gr.names = (char**)malloc(32 * sizeof(char*));
	gr.name_count = 0;
	size_t capasity = 32;
	
	char buff[64];
	memset(buff, 0, 64);

	while(fgets(buff, 64, file)){
		char* new_line = (char*)malloc(strlen(buff));
		strcpy(new_line, buff); new_line[strlen(buff)-1] = '\0';
		if(gr.name_count == capasity){
			capasity += 32;
			char** help_wort = (char**)malloc(sizeof(char*) * capasity);
			size_t i = 0;
			while(i < gr.name_count) {
				help_wort[i] = gr.names[i];
				i++;
			}
			free(gr.names);
			gr.names = help_wort;
		}
		gr.names[gr.name_count++] = new_line;
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
		size_t i, j;
		memset(buff, 0, 64);
		read_lexem(reader, buff, 64, '+');
		for(i = lib.group_count; i--;)
			for(j = lib.groups[i].name_count; j--;)
				if(strstr(lib.groups[i].names[j], buff)) lib.recepts[lib.recept_count].reagent1 = lib.groups[i].names[j];
		memset(buff, 0, 64);
		read_lexem(reader, buff, 64, '=');
		for(i = lib.group_count; i--;)
			for(j = lib.groups[i].name_count; j--;)
				if(strstr(lib.groups[i].names[j], buff)) lib.recepts[lib.recept_count].reagent2 = lib.groups[i].names[j];
		memset(buff, 0, 64);
		read_lexem(reader, buff, 64, '\n');
		for(i = lib.group_count; i--;)
			for(j = lib.groups[i].name_count; j--;)
				if(strstr(lib.groups[i].names[j], buff)) lib.recepts[lib.recept_count].rezult = lib.groups[i].names[j];
	}
    fclose(reader);
	return lib;
}
