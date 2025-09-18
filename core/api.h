#ifndef API_H
#define API_H

#include <stdbool.h>

bool new_game(const char* path);
bool save(const char* path);
bool load(const char* path);
bool game_exit();

typedef struct{
    char* reagent1;
    char* reagent2;
    char* result;
} combination;

char* list_elements();
char* list_groups();
char* look_group(const char* group);
char* partial_match_groups(const char* group);
char* partial_match_elements(const char* element);
char* get_rest();

char* status();
char* check_combination(const char* elem1, const char* elem2);

combination list_combinations();
combination find_combinations_for_element(const char* element);
combination next_combination();

// For tests //

void openAll();

#endif // API_H
