#include "game.h"

char* dualisation(library* lib, char* element1, char* element2){
	int i;
	for(i = lib->recept_count; i--;){
		combinate* com = lib->recepts + i;
		if((com->reagent1 == element1 && com->reagent2 == element2) || (com->reagent1 == element2 && com->reagent2 == element1)){
			char* ptr = com->rezult;
			if(ptr[0] >> 7) ptr[0] ^= 1<<7;
			return ptr;
		}
	}
	return 0;
}

