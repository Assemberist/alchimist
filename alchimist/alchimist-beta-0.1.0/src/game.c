#include "game.h"

char* dualisation(library* lib, char* element1, char* element2){
	int i;
	for(i = lib->recept_count; i--;){
		if(lib->recepts[i].reagent1 == element1){
			if(lib->recepts[i].reagent2 == element2)
				return lib->recepts[i].rezult;
		}
		else if(lib->recepts[i].reagent1 == element2){
			if(lib->recepts[i].reagent2 == element1)
				return lib->recepts[i].rezult;
		}
	}
	return 0;
}

