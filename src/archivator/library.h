#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../submod/string_tree/src/string_tree.h"

#include <stddef.h>
#include <stddef.h>

#include <dirent.h>
#include <unistd.h>

#ifdef __linux__
#include <linux/limits.h>

#elifdef _WIN32

#endif

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
} library;

library load_library(char* path);
token* wordbook_from_library(library* lib);
void dispose_library(library* lib);

#endif
