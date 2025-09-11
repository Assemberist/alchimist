#include "api.h"
#include "alch_types.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "string_tree/pack.h"

///////////////////////////////////////

element_t* elements = NULL;
uint16_t element_num = 0;
combination_t* combinations = NULL;
uint16_t combination_num = 0;

pack p;

char* group_names;
char* element_names;

#define META_LEN sizeof(uint16_t) * 2 - sizeof(uint32_t) * 3

///////////////////////////////////////

bool new_game(const char* path){
    // Cleanup previous game
    if(elements){
        free(elements);
        elements = NULL;
    }

    FILE* in = fopen(path, "rb");
    if(in){
        uint32_t group_offset;
        uint32_t element_offset;

        fread(&element_num, sizeof(uint16_t), 1, in);
        fread(&combination_num, sizeof(uint16_t), 1, in);
        fread(&p.info.nodes, sizeof(uint32_t), 1, in);
        fread(&group_offset, sizeof(uint32_t), 1, in);
        fread(&element_offset, sizeof(uint32_t), 1, in);

        fseek(in, 0, SEEK_END);
        size_t size = ftell(in) - META_LEN;

        if(!(elements = malloc(size))) return false;

        fseek(in, META_LEN, SEEK_SET);
        fread(&elements, sizeof(uint32_t), 1, in);

        return true;
    }
    else return false;
}

/*bool save(const char* path);
bool load(const char* path);
bool game_exit();

char* status();
char* list_groups();
char* look_group(const char* group);
char* match_elements(int limit, char* pattern);
char* check_combination(const char* elem1, const char* elem2);*/