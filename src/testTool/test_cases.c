#include "int_check.h"

#define SUCCESS 0;
#define ERROR 1;
// заглушки

int undiscovered(library* lib){return 0;}
int unused(library* lib){return 0;}
int unget(library* lib){return 0;}
int again_elements(library* lib){return 0;}
int again_combinate(library* lib){return 0;}

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

/*
void find_element_in_andere_groups(library lib, int group_num, int name_num){
	char* src = lib.groups[group_num].names[name_num].value;
	int i = group_num;
	while(i-- > 0){
		int j = lib.groups[i].name_count;
		while(j-- > 0){
			if(!strcmp(lib.groups[i].names[j].value, src)){
				printf(
					"multiple input element: %s in group %s and group %s\n",
					lib.groups[i].names[j].value,
					lib.groups[group_num].name,
					lib.groups[i].name
				);
				return;
			}
		}
	}
}

void open_elements(library lib){
	int i = lib.group_count;
	while(i--){
		int j = lib.groups[i].name_count;
		while(j-- > 1){
			lib.groups[i].names[j].value[0] &= 127;
		}
	}
}

void again_elements(library lib){
	open_elements(lib);
	int i = lib.group_count;
	while(i--){
		int j = lib.groups[i].name_count;
		while(j-- > 0){
			int k = j;
			while(k-- > 0){
				if(!strcmp(lib.groups[i].names[j].value, lib.groups[i].names[k].value)){
					printf(
						"multiple input element: %s in group %s and group %s\n",
						lib.groups[i].names[j].value,
						lib.groups[i].name,
						lib.groups[i].name
					);
					goto next_element;
				}
			}
			find_element_in_andere_groups(lib, i, j);
			next_element: continue;
		}
	}
}

void again_combinate(library lib){
	int i = lib.recept_count;
	while(--i){
		int j = i-1;
		while(j--){
			if(
				(lib.recepts[i].reagent1 == lib.recepts[j].reagent1 &&
				lib.recepts[i].reagent2 == lib.recepts[j].reagent2) ||
				(lib.recepts[i].reagent1 == lib.recepts[j].reagent2 &&
				lib.recepts[i].reagent2 == lib.recepts[j].reagent1)
			) {
				printf("has collision for combination: %s + %s\n", lib.recepts[i].reagent1->value, lib.recepts[i].reagent2->value);
				goto nextcom;
			}
		}
		nextcom: continue;
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