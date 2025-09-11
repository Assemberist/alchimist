#ifndef API_H
#define API_H

#include <stdbool.h>

bool new_game(const char* path);
bool save(const char* path);
bool load(const char* path);
bool game_exit();

char* status();
char* list_groups();
char* look_group(const char* group);
char* match_elements(int limit, char* pattern);
char* check_combination(const char* elem1, const char* elem2);


// For test

#include "string_tree/pack.h"
#include "alch_types.h"

extern element_t* elements;
extern uint16_t element_num;
extern combination_t* combinations;
extern uint16_t combination_num;
extern pack p;
extern char* group_names;
extern char* element_names;


#endif // API_H
