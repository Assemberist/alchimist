#include "api.h"

int main(int argc, char** argv){ return argc >= 2 ? new_game(argv[1]) : 0; }