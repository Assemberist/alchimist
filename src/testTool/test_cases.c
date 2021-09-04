#include "int_check.h"

#define SUCCESS 0;
#define ERROR 1;
// заглушки

int undiscovered(library* lib){return 0;}
int unused(library* lib){return 0;}
int unget(library* lib){return 0;}

int missing(library* lib){
	int flag = SUCCESS;
	int i = lib->recept_count;
	while(i--){
		combinate* com = lib->recepts + i;
		char *names[3] = {
			com->reagent1 ? get_el_name(com->reagent1) : NULL,
			com->reagent2 ? get_el_name(com->reagent2) : NULL,
			com->rezult ? get_el_name(com->rezult) : NULL
		};
		if(names[0] && names[1] && names[2]) continue;

		flag = ERROR;
		printf(
			"Record corrupted: %s + %s = %s\n",
			names[0] ? names[0] : "???",
			names[1] ? names[1] : "???",
			names[2] ? names[2] : "???"
		);
	}

	return flag;
}

int again_elements(library* lib){
	int flag = SUCCESS;
	size_t i, j;
	for(i = lib->group_count; i--;){
		for(j = lib->groups[i].name_count; j--;){
			element* val = lib->groups[i].names + j;
			if(find_element(get_el_name(val), lib->worterbuch) != val){
				printf("Duplicate of element found: %s::%s\n", lib->groups[i].name, get_el_name(val));
				flag = ERROR;
			}
		}
	}

	return flag;
}

int again_combinate(library* lib){
	int flag = SUCCESS;
	size_t i, j;
	for(i = lib->recept_count; --i;){
		combinate* com_i = lib->recepts + i;
		for(j = i; j--;){
			combinate* com_j = lib->recepts + j;
			if(	(com_i->reagent1 == com_j->reagent1 && com_i->reagent2 == com_j->reagent2) ||
				(com_i->reagent1 == com_j->reagent2 && com_i->reagent2 == com_j->reagent1)
			){
				printf("Duplicat of recept found: %s::%s\n",
						get_el_name(com_i->reagent1),
						get_el_name(com_i->reagent2));

				flag = ERROR;
			}
		}
	}

	return flag;
}

/*

void open_elements(library lib){
	int i = lib.group_count;
	while(i--){
		int j = lib.groups[i].name_count;
		while(j-- > 1){
			lib.groups[i].names[j].value[0] &= 127;
		}
	}
}

void unused(library lib){
	int i = lib.group_count;	
	while(i--){
		int j = lib.groups[i].name_count;
		while(j--){
			int s = lib.recept_count;
			while(s--){
				char* value = lib.groups[i].names[j].value;
				if(value == lib.recepts[s].reagent1->value) goto nextname;
				if(value == lib.recepts[s].reagent2->value) goto nextname;
				if(value == lib.recepts[s].rezult->value) goto nextname;
			}
			char ch = lib.groups[i].names[j].value[0] & 128;
			lib.groups[i].names[j].value[0] &= 127;
			printf("unusing element %s in group %s\n", lib.groups[i].names[j].value, lib.groups[i].name);
			lib.groups[i].names[j].value[0] |= ch;
			nextname: continue;
		}
	}
}

void undiscovered(library lib){
	int counter = 0;
	int i = lib.group_count;
	while(i--){
		int j = lib.groups[i].name_count;
		while(j--){
			if(!(lib.groups[i].names[j].value[0] & 128)) continue;
			int s = lib.recept_count;
			while(s--) if(lib.groups[i].names[j].value == lib.recepts[s].rezult->value) goto nextname;
			lib.groups[i].names[j].value[0] ^= 128;
			printf("undiscovered element %s in group %s\n", lib.groups[i].names[j].value, lib.groups[i].name);
			lib.groups[i].names[j].value[0] ^= 128;
			nextname: continue;
		}
	}
}

void unget(library lib){
	int i;
	int disc = 1;
	int last = lib.recept_count - 1;
	
	for(i=0; i <= last; i++){
		if(!(
			lib.recepts[i].reagent1->value[0] & 128 ||
			lib.recepts[i].reagent2->value[0] & 128 ||
			lib.recepts[i].rezult->value[0] & 128
		)) {
			combinate com = lib.recepts[i];
			lib.recepts[i] = lib.recepts[last];
			lib.recepts[last] = com;
			last--;
			i--;
		}
	}
	
	while(last >= 0 && disc){
		disc = 0;
		for(i=0; i <= last; i++){
			if(!(
				lib.recepts[i].reagent1->value[0] & 128 ||
				lib.recepts[i].reagent2->value[0] & 128 
			)) {
				lib.recepts[i].rezult->value[0] &= 127;
				combinate com = lib.recepts[i];
				lib.recepts[i] = lib.recepts[last];
				lib.recepts[last] = com;
				disc = 1;
				last--;
				i--;
			}
		}
	}

	if(last >= 0){
		for(i=0; i<last+1; i++){
			lib.recepts[i].reagent1->value[0] &= 127;
			lib.recepts[i].reagent2->value[0] &= 127;
			lib.recepts[i].rezult->value[0] &= 127;
			printf(
				"asimptote combinate: %s + %s = %s\n",
				lib.recepts[i].reagent1->value,
				lib.recepts[i].reagent2->value,
				lib.recepts[i].rezult->value
			);
		}
	}
}


*/