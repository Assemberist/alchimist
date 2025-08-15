#include "parser.h"

char* get_combi_path(char* path){
    char* ptr = (char*)malloc(strlen(path)+strlen("/combinations.txt")+1);
    sprintf(ptr, "%s%s\0", path, "/combinations.txt");
    return ptr;
}

char* get_group_path(char* path){
    char* ptr = (char*)malloc(strlen(path)+strlen("/groups")+1);
    sprintf(ptr, "%s%s\0", path, "/groups");
    puts(ptr);
    return ptr;
}
