#include "loader.h"
#include "render.h"

int main(){
	library lib=load_library();
	puts("yey");
	draw(&lib);
	return 0;
}
