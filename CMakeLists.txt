add_executable(archivator.exe archivator.c)
add_executable(validator.exe validator.c)

add_compile_options(-g)

add_library(stringTree OBJECT submod/string_tree/string_tree.c)

FILE(GLOB Core src/core/*.c)
add_library(CoreObj OBJECT ${Core})


add_executable(game.exe $<TARGET_OBJECTS:stringTree> $<TARGET_OBJECTS:CoreObj>)