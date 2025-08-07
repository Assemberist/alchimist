#include "loader.h"

int main(int argc, char** argv){
    char* path = "../alchimist/lib example";

    library lib = load_library(path);

    //if(argc == 2) lib = load_library(argv[1]);
    //else return -1;

    puts(lib.groups[0].name);
}
