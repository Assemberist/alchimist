#include "loader.h"
#include "render.h"
#include "int_check.h"

int main(){
	library lib = load_library();

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
