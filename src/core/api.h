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

#endif // API_H
