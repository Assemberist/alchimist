#include "render.h"

#define CONSOLE

#ifdef CONSOLE

char* choose_element(library* lib){
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

	puts(buff);

	puts("choose element");
	for(j = 0; j < lib->groups[i].name_count; j++) 
		if(!(lib->groups[i].names[j][0] & 1<<7))
			printf("%i) %s\n", j+1, lib->groups[i].names[j]);
	
	gets(buff);
	for(j = lib->groups[i].name_count; j--;)
		if(strstr(lib->groups[i].names[j], buff)) break;
	
	if(j == -1) return 0;

	return lib->groups[i].names[j];
}

void draw(library* lib){
	char *first, *twice, *rez;
	
	system("clear");
	puts("first element");
	if(!(first = choose_element(lib))){
		puts("wrong name");
		return;
	}
	printf("you chhose %s\n", first);

	puts("twise element");
	if(!(twice = choose_element(lib))){
		puts("wrong name");
		return;
	}

	printf("you choose %s\n", twice);
	system("clear");
	printf("%s + %s = ", first, twice);
	rez = dualisation(lib, first, twice);

	if(!(rez)){
		puts("wrong combinate");
		return;
	}
	printf("you get %s\n", rez);

}

#endif

#ifdef XLIB

void draw(library* lib){

}

#endif
