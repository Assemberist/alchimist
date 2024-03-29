#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "../service/string_tree.h"

typedef union element{
	struct{
		char* name;
	} longName;
	struct{
		char name[sizeof(char*) * 2 - 1];
		uint8_t is_long:1;
		uint8_t is_open:7;
	} shortName;
}element;

char* get_el_name(element* ptr);

typedef struct group{
	char* name;
	element* names;
	size_t name_count;
}group;

typedef struct combinate{
	element* reagent1;
	element* reagent2;
	element* rezult;
}combinate;

typedef struct library{
	combinate* recepts;
	size_t recept_count;
	group* groups;
	size_t group_count;
	token* worterbuch;
}library;

void* read_file(char* path, void(*parser)(char*, void*, void*), size_t element_size, void* args, size_t* rows);
char** search_libs(const char* lib_foulder, size_t* len);
element* dualisation(library* lib, element* element1, element* element2, size_t session);
void dispose_library(library* lib);

#endif
