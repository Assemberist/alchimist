#ifndef API_H
#define API_H

#include <stdbool.h>

bool new_game(const char* path);
bool save(const char* path);
bool load(const char* path);
bool game_exit();

char* status();
char* list_groups();
char* list_combinations();
char* look_group(const char* group);
char* partial_match_groups(const char* group);
char* partial_match_elements(const char* element);
char* find_combinations_for_element(const char* element);
char* check_combination(const char* elem1, const char* elem2);
char* get_rest();

#endif // API_H
