#ifndef INT_CHECK_H
#define INT_CHECK_H

#include <string.h>
#include "../core/loader.h"

typedef union element_ext{
	struct{
		char* name;
	} longName;
	struct{
		char name[sizeof(char*) * 2 - 1];
		uint8_t is_open:1;
		uint8_t is_long:1;
        uint8_t initial_value:1;
	} shortName;
}element_ext;

// test cases
int missing(library* lib);
int undiscovered(library* lib);
int unused(library* lib);
int again_elements(library* lib);
int again_combinate(library* lib);

#endif
