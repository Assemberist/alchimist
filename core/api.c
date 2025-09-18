#include "api.h"
#include "alch_types.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "string_tree/pack.h"

#include "bycicles.h"

#define META_LEN (sizeof(uint16_t) * 2 + sizeof(uint32_t) * 3)

///////////////////////////////////////
// static vars //
///////////////////////////////////////

element_t* elements = NULL;
uint16_t element_num = 0;
combination_t* combinations = NULL;
uint16_t combination_num = 0;

pack p;

char* group_names;
char* element_names;

///////////////////////////////////////
// static vars //
///////////////////////////////////////

enum request_type {
    LIST_GROUPS,
    LIST_ELEMENTS,
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
static size_t current_element = 0;

///////////////////////////////////////
// help functions //
///////////////////////////////////////

bool isComboOpen(size_t id){
    return elements[combinations[counter].id1].openFlag &&
           elements[combinations[counter].id2].openFlag &&
           elements[combinations[counter].idResult].openFlag;
}

#define returnCombination(A) \
            return (combination){ \
                    element_names + elements[combinations[A].id1].namePos, \
                    element_names + elements[combinations[A].id2].namePos, \
                    element_names + elements[combinations[A].idResult].namePos }

///////////////////////////////////////
// test section //
///////////////////////////////////////

void openAll(){
    for(size_t i = 0; i < element_num; i++)
        elements[i].openFlag = true;
}

///////////////////////////////////////
// API calls //
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

        return true;
    }
    else return false;
}

char* list_groups(){
    latest_request = LIST_GROUPS;
    current_group = NULL;
    for(counter = 0; counter < element_num; counter++)
        if(elements[counter].openFlag)
            return current_group = group_names + elements[counter].groupNamePos;

    latest_request = NOTHING;
    return NULL;
}

char* list_elements(){
    latest_request = LIST_ELEMENTS;
    for(counter = 0; counter < element_num; counter++)
        if(elements[counter].openFlag)
            return element_names + elements[counter].namePos;

    latest_request = NOTHING;
    return NULL;
}

char* look_group(const char* group){
    latest_request = LOOK_GROUP;
    size_t gp;

    for(counter = 0; counter < element_num; counter++)
        if(!strcmp(group, group_names + elements[counter].groupNamePos)){
            gp = elements[counter].groupNamePos;
            goto group_check;
        }

    for(; counter < element_num; counter++)
        if(elements[counter].groupNamePos == gp)
            group_check:
                if(elements[counter].openFlag)
                    return element_names + elements[counter].namePos;

    latest_request = NOTHING;
    return NULL;
}

/*bool save(const char* path);
bool load(const char* path);
bool game_exit();

char* partial_match_groups(const char* group);
char* partial_match_elements(const char* element);
char* get_rest();

char* status();
char* check_combination(const char* elem1, const char* elem2);
*/

char* get_rest(){
    counter++;
    switch(latest_request){
        case LIST_GROUPS:
            for(; counter < element_num; counter++){
                if(elements[counter].openFlag){
                    char* gp = group_names + elements[counter].groupNamePos;
                    if(gp != current_group){
                        current_group = gp;
                        return gp;
                    }
                }
            }
            break;

        case LIST_ELEMENTS:
            for(; counter < element_num; counter++)
                if(elements[counter].openFlag)
                    return element_names + elements[counter].namePos;

            break;

        case LOOK_GROUP:
            for(; counter < element_num; counter++){
                if(elements[counter].groupNamePos != elements[counter-1].groupNamePos)
                    break;

                if(elements[counter].openFlag)
                    return element_names + elements[counter].namePos;
            }

        default:
            break;
    }
    
    latest_request = NOTHING;
    return NULL;
}

combination list_combinations(){
    latest_request = LIST_COMBINATIONS;
    counter = -1;
    return next_combination();
}

combination find_combinations_for_element(const char* element){
    latest_request = FIND_ELEMENT_COMBO;
    current_element = (size_t)find_pack_element(element, p);

    if(!elements[current_element].openFlag)
        return (combination){NULL, NULL, NULL};

    for(counter = 0; counter < combination_num; counter++){
        if(combinations[counter].id1 == current_element ||
           combinations[counter].id2 == current_element){
                if(isComboOpen(counter))
                    returnCombination(counter);
        }
    }
    return (combination){NULL, NULL, NULL};
}

combination next_combination(){
    counter++;
    switch(latest_request){
        case LIST_COMBINATIONS:
            for(; counter < combination_num; counter++)
                if(isComboOpen(counter))
                    returnCombination(counter);

            break;

        case FIND_ELEMENT_COMBO:
            for(; counter < combination_num; counter++)
                if( combinations[counter].id1 == current_element ||
                    combinations[counter].id2 == current_element)
                        if(isComboOpen(counter))
                            returnCombination(counter);

        default:
            break;
    }

    latest_request = NOTHING;
    return (combination){NULL, NULL, NULL};
}