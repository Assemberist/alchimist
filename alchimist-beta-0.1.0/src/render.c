#include "render.h"

#define CONSOLE

#ifdef CONSOLE

element* choose_element(library* lib){
	size_t i, j;
	char buff[64];

	puts("choose group\n");
	for(i = 0; i < lib->group_count; i++) 
		printf("%i) %s\n", i+1, lib->groups[i].name);

	gets(buff);
	for(i = lib->group_count; i--;)
		if(strstr(lib->groups[i].name, buff)) break;

	if(i == -1){
		system("clear");
		puts("wrong name");
		return 0;
	}

	puts(lib->groups[i].name);

	puts("choose element");
	for(j = 0; j < lib->groups[i].name_count; j++) 
		if(lib->groups[i].names[j].is_open)
			printf("%i) %s\n", j+1, lib->groups[i].names[j].value);
	
	gets(buff);
	for(j = lib->groups[i].name_count; j--;)
		if(lib->groups[i].names[j].is_open)
			if(strstr(lib->groups[i].names[j].value, buff)) return lib->groups[i].names + j;
	
	return 0;
}

void draw(library* lib){
	element *first, *twice, *rez;
	
	puts("first element");
	if(!(first = choose_element(lib))){
		puts("wrong name");
		return;
	}
	printf("you chhose %s\n", first->value);

	puts("twise element");
	if(!(twice = choose_element(lib))){
		puts("wrong name");
		return;
	}

	printf("you choose %s\n", twice->value);
	system("clear");
	printf("%s + %s = ", first->value, twice->value);

	rez = dualisation(lib, first, twice);
	rez ? printf("you get %s\n", rez->value) : puts("wrong combinate");	
}

#endif

#ifdef XLIB

void draw(library* lib){

}

#endif
