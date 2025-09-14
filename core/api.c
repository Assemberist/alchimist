#include "api.h"
#include "alch_types.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "string_tree/pack.h"

#define META_LEN (sizeof(uint16_t) * 2 + sizeof(uint32_t) * 3)

///////////////////////////////////////

element_t* elements = NULL;
uint16_t element_num = 0;
combination_t* combinations = NULL;
uint16_t combination_num = 0;

pack p;

char* group_names;
char* element_names;

///////////////////////////////////////

enum request_type {
    LIST_GROUPS,
    LIST_COMBINATIONS,
    LOOK_GROUP,
    PARTIAL_MATCH_GROUPS,
    PARTIAL_MATCH_ELEMENTS,
    FIND_ELEMENT_COMBO,
    CHECK_COMBINATION,
    NOTHING
} latest_request = NOTHING;

size_t counter = 0;

static char* current_group = NULL;

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

        // allocate memory and load archive here
        // elements
        if(!(elements = malloc(size))) return false;
        fseek(in, META_LEN, SEEK_SET);
        fread(elements, size, 1, in);

        // combinations
        combinations = (combination_t*)(elements + element_num);

        // pack
        p.values = (void**)(combinations + combination_num);
        p.text_shifts = (uint32_t*)(p.values + p.info.nodes);
        p.flags = (uint8_t*)(p.text_shifts + p.info.nodes);
        p.texts = (char*)(p.flags + p.info.nodes / 4);

        // texts
        group_names = (char*)elements + group_offset;
        element_names = (char*)elements + element_offset;

        for(counter = 0; counter < element_num; counter++)
            printf("%s:%s %d->%s\n",
                p.texts + elements[counter].groupNamePos,
                p.texts + elements[counter].namePos,
                p.texts + elements[counter].namePos,
                elements[counter].openFlag ? "[OPEN]" : ""
            );

        return true;
    }
    else return false;
}

char* list_groups(){
    printf("%d -> air\n\n", elements);

    latest_request = LIST_GROUPS;
    current_group = NULL;
    for(counter = 0; counter < element_num; counter++){
        printf("%s:%s %d->%s\n",
               p.texts + elements[counter].groupNamePos,
               p.texts + elements[counter].namePos,
               p.texts + elements[counter].namePos,
               elements[counter].openFlag ? "[OPEN]" : ""
            );

        if(elements[counter].openFlag){
            current_group = p.texts + elements[counter].groupNamePos;
            break;
        }
    }
    return current_group;
}


/*bool save(const char* path);
bool load(const char* path);
bool game_exit();

char* status();
char* look_group(const char* group);
char* match_elements(int limit, char* pattern);
char* check_combination(const char* elem1, const char* elem2);
*/

char* get_rest(){
    switch(latest_request){
        case LIST_GROUPS:
            for(; counter < element_num; counter++){
                if(elements[counter].openFlag){
                    char* gp = p.texts + elements[counter].groupNamePos;
                    if(gp != current_group){
                        current_group = gp;
                        return gp;
                    }
                }
            }
        
            current_group = NULL;
            latest_request = NOTHING;
            return current_group;


        default:
            return NULL;
    }
}