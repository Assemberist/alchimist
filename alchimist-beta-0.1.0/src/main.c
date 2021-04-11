#include "loader.h"
#include "render.h"
#include "int_check.h"
#include "string_tree.h"

int main(){
	library lib = load_library("/home/sanya/Рабочий стол/sources/alchimist/alchimist-beta-0.1.0/debug");

	draw(&lib);

	return 0;
}
