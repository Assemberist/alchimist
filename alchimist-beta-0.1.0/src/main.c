#include "loader.h"
#include "render.h"
#include "int_check.h"
#include "string_tree.h"

int main(){
	library lib = load_library("/home/sanya/Рабочий стол/sources/alchimist/alchimist-beta-0.1.0/debug");

	int i;
	for(i = lib.recept_count; i--;){
		printf(lib.recepts[i].reagent1);
		putchar('+');
		printf(lib.recepts[i].reagent2);
		putchar('=');
		puts(lib.recepts[i].rezult);
	}
	return 0;
}
