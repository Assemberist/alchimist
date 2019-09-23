#include "loader.h"
#include "render.h"

int main(){
	library lib=load_library();
	draw(&lib);
	return 0;
}
