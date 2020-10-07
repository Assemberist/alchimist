#include "int_check.h"

int missing(library lib);
void undiscovered(library lib);
void unused(library lib);
void unget(library lib);
void again_elements(library lib);
void again_combinate(library lib);

void check_library(){
	library lib = load_library("/home/sanya/Рабочий стол/sources/alchimist/alchimist-beta-0.1.0/debug");
	if(missing(lib)) {
		puts("this is critical!");
		puts("check abort");
		return;
	}
	puts("done[1/6]");
	unused(lib);
	puts("done[2/6]");
	undiscovered(lib);
	puts("done[3/6]");
	unget(lib);
	puts("done[4/6]");
	again_elements(lib);
	puts("done[5/6]");
	again_combinate(lib);
	puts("done[6/6]");

}

void find_element_in_andere_groups(library lib, int group_num, int name_num){
	char* src = lib.groups[group_num].names[name_num];
	int i = group_num;
	while(i-- > 0){
		int j = lib.groups[i].name_count;
		while(j-- > 0){
			if(!strcmp(lib.groups[i].names[j], src)){
				printf(
					"multiple input element: %s in group %s and group %s\n",
					lib.groups[i].names[j],
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
			lib.groups[i].names[j][0] &= 127;
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
				if(!strcmp(lib.groups[i].names[j], lib.groups[i].names[k])){
					printf(
						"multiple input element: %s in group %s and group %s\n",
						lib.groups[i].names[j],
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
				printf("has collision for combination: %s + %s\n", lib.recepts[i].reagent1, lib.recepts[i].reagent2);
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
				if(lib.groups[i].names[j] == lib.recepts[s].reagent1) goto nextname;
				if(lib.groups[i].names[j] == lib.recepts[s].reagent2) goto nextname;
				if(lib.groups[i].names[j] == lib.recepts[s].rezult) goto nextname;
			}
			char ch = lib.groups[i].names[j][0] & 128;
			lib.groups[i].names[j][0] &= 127;
			printf("unusing element %s in group %s\n", lib.groups[i].names[j], lib.groups[i].name);
			lib.groups[i].names[j][0] |= ch;
			nextname: continue;
		}
	}
}

int missing(library lib){
	int counter = 0;
	int i = lib.recept_count;
	while(i--){
		if(lib.recepts[i].reagent1 && lib.recepts[i].reagent2 && lib.recepts[i].rezult) continue;

		int reagents[3] = {0,0,0};
		if(lib.recepts[i].reagent1 && lib.recepts[i].reagent1[0] & 128) { 
			lib.recepts[i].reagent1[0] ^= 128;
			reagents[0] = 1;
		}
		if(lib.recepts[i].reagent2 && lib.recepts[i].reagent2[0] & 128) {
			lib.recepts[i].reagent2[0] ^= 128;
			reagents[1] = 1;
		}
		if(lib.recepts[i].rezult && lib.recepts[i].rezult[0] & 128) {
			lib.recepts[i].rezult[0] ^= 128;
			reagents[2] = 1;
		}
		printf(
			"breaked combinate: %s + %s = %s\n",
			(lib.recepts[i].reagent1 ? lib.recepts[i].reagent1 : "???"),
			(lib.recepts[i].reagent2 ? lib.recepts[i].reagent2 : "???"),
			(lib.recepts[i].rezult ? lib.recepts[i].rezult : "???")
		);
		if(reagents[0]) lib.recepts[i].reagent1[0] ^= 128;
		if(reagents[0]) lib.recepts[i].reagent2[0] ^= 128;
		if(reagents[0]) lib.recepts[i].rezult[0] ^= 128;
		counter++;
	}
	return counter;
}

void undiscovered(library lib){
	int counter = 0;
	int i = lib.group_count;
	while(i--){
		int j = lib.groups[i].name_count;
		while(j--){
			if(!(lib.groups[i].names[j][0] & 128)) continue;
			int s = lib.recept_count;
			while(s--) if(lib.groups[i].names[j] == lib.recepts[s].rezult) goto nextname;
			lib.groups[i].names[j][0] ^= 128;
			printf("undiscowered element %s in group %s\n", lib.groups[i].names[j], lib.groups[i].name);
			lib.groups[i].names[j][0] ^= 128;
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
			lib.recepts[i].reagent1[0] & 128 ||
			lib.recepts[i].reagent2[0] & 128 ||
			lib.recepts[i].rezult[0] & 128
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
				lib.recepts[i].reagent1[0] & 128 ||
				lib.recepts[i].reagent2[0] & 128 
			)) {
				lib.recepts[i].rezult[0] &= 127;
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
			lib.recepts[i].reagent1[0] &= 127;
			lib.recepts[i].reagent2[0] &= 127;
			lib.recepts[i].rezult[0] &= 127;
			printf(
				"asimptote combinate: %s + %s = %s\n",
				lib.recepts[i].reagent1,
				lib.recepts[i].reagent2,
				lib.recepts[i].rezult
			);
		}
	}
}


