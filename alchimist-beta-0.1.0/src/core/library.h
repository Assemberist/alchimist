#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef union element{
	struct{
		char* name;
		char _pad[sizeof(char*) - 1];
		uint8_t is_open:1;
		uint8_t is_long:1;		
	} longName;
	struct{
		char name[sizeof(char*) * 2 - 1];
		uint8_t is_open:1;
		uint8_t is_long:1;
	} shortName;
}element;

element create_element(char* src);
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
}library;

element* dualisation(library* lib, element* element1, element* element2);
void dispose_library(library* lib);

#endif