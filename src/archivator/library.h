#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "../submod/string_tree/src/string_tree.h"

typedef struct element{
	char* name;
	uint8_t is_open:1;
} element;

typedef struct group{
	char* name;
	element* names;
	size_t name_count;
} group;

typedef struct combinate{
	element* reagent1;
	element* reagent2;
	element* rezult;
} combinate;

typedef struct library{
	combinate* recepts;
	size_t recept_count;
	group* groups;
	size_t group_count;
	token* worterbuch;
} library;

void* read_file(char* path, void(*parser)(char*, void*, void*), size_t element_size, void* args, size_t* rows);
void dispose_library(library* lib);

#endif
