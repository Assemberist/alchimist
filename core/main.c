#include "api.h"
#include <stdio.h>

int main(int argc, char** argv){ 
    if(argc == 1) return -1;

    if(new_game(argv[1])){    
        char* ptr = list_groups();

        puts(ptr ? ptr : "NULL");
        while(ptr = get_rest()) puts(ptr);
    }
}