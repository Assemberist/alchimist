#include "api.h"
#include "alch_types.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "string_tree/pack.h"

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

char partial_name[40];

struct{
    uint32_t* nodes;
    size_t depth;
} node_stack;

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

int cmpCombo(const void* c1, const void* c2){ return *(uint32_t*)c1 == *(uint32_t*)c2; }

void create_node_stack(pack package){
    // find max depth
    // size_t max_depth =

    // node_stack.nodes = malloc(max_depth * sizeof(uint32_t));
}

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

        create_node_stack(p);

        return true;
    }
    else return false;
}

char* list_groups(){
    latest_request = LIST_GROUPS;
    current_group = NULL;
    counter = -1;
    return get_rest();
}

char* list_elements(){
    latest_request = LIST_ELEMENTS;
    counter = -1;
    return get_rest();
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

char* partial_match_groups(const char* group){
    latest_request = PARTIAL_MATCH_GROUPS;
    current_group = 0;
    counter = -1;
    strcpy(partial_name, group);
    return get_rest();
}

char* partial_match_elements(const char* element){
    latest_request = PARTIAL_MATCH_ELEMENTS;


    return get_rest();
}

/*bool save(const char* path);
bool load(const char* path);
bool game_exit();

char* status();
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
            break;

        case PARTIAL_MATCH_GROUPS:
            for(; counter < element_num; counter++){
                if(elements[counter].openFlag){
                    char* gp = group_names + elements[counter].groupNamePos;
                    if(gp != current_group){
                        current_group = gp;
                        if(strstr(current_group, partial_name))
                            return current_group;
                    }
                }
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
    current_element = (size_t)find_pack_element(element, p, (void*)UINT64_MAX);
    
    if(current_element == UINT64_MAX)
        return (combination){NULL, NULL, NULL};

    if(!elements[current_element].openFlag)
        return (combination){NULL, NULL, NULL};

    for(counter = 0; counter < combination_num; counter++){
        if(combinations[counter].id1 == current_element ||
           combinations[counter].id2 == current_element){
                if(isComboOpen(counter))
                    returnCombination(counter);
        }
    }

    latest_request = NOTHING;
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

char* check_combination(const char* elem1, const char* elem2){
    size_t id1 = (size_t)find_pack_element(elem1, p, (void*)UINT64_MAX);
    if(id1 == UINT64_MAX) return NULL;
    if(!elements[id1].openFlag) return NULL;

    size_t id2 = (size_t)find_pack_element(elem2, p, (void*)UINT64_MAX);
    if(id2 == UINT64_MAX) return NULL;
    if(!elements[id2].openFlag) return NULL;

    uint32_t combo = id1 > id2 ? (id2 << 16) | id1 : (id1 << 16) | id2;

    void* value = bsearch(&combo,
                         combinations,
                        combination_num,
                         sizeof(combination_t),
                       cmpCombo);
    
    if(!value) return NULL;

    element_t* elem = elements + ((combination_t*)value)->idResult;
    elem->openFlag = true;

    return element_names + elem->namePos;
}
