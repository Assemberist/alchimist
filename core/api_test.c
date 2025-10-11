#include "api.h"
#include <stdio.h>

/*
char* partial_match_groups(const char* group);
char* partial_match_elements(const char* element);
char* check_combination(const char* elem1, const char* elem2);
combination find_combinations_for_element(const char* element);
*/

int main(int argc, char** argv){ 
    if(argc == 1) return -1;

    if(new_game(argv[1])){
        /*
        {
            puts("Element list:");
            for(char* ptr = list_elements(); ptr; ptr = get_rest())
                puts(ptr);

            puts("\nGroup list:");
            size_t i = 0;
            for(char* ptr = list_groups(); ptr; ptr = get_rest()){
                puts(ptr);
                i++;
            }

            char* groups[i];
            i = 0;
            for(char* ptr = list_groups(); ptr; ptr = get_rest())
                groups[i++] = ptr;

            puts("\nLook groups:");
            for(int j = 0; j < i; j++){
                puts(groups[j]);
                for(char* ptr = look_group(groups[j]); ptr; ptr = get_rest())
                    puts(ptr);
            }

            puts("\nList combinations");
            for(combination combo = list_combinations(); combo.reagent1; combo = next_combination())
                printf("%s + %s = %s\n", combo.reagent1, combo.reagent2, combo.result);

            puts("\nCheck combinations");
            for(combination combo = list_combinations(); combo.reagent1; combo = next_combination()){
                char* ptr = check_combination(combo.reagent1, combo.reagent2);
                printf("Original: %s + %s = %s\n", combo.reagent1, combo.reagent2, ptr ? ptr : "<<Null>>");
            }
        }
*/
        save("test", true);

        puts("\nOpen all\n");
        openAll();

        {
            puts("\nElement list:");
            for(char* ptr = list_elements(); ptr; ptr = get_rest())
                puts(ptr);

            puts("\nGroup list:");
            size_t i = 0;
            for(char* ptr = list_groups(); ptr; ptr = get_rest()){
                puts(ptr);
                i++;
            }

            char* groups[i];
            i = 0;
            for(char* ptr = list_groups(); ptr; ptr = get_rest())
                groups[i++] = ptr;

            puts("\nLook groups:");
            for(int j = 0; j < i; j++){
                puts(groups[j]);
                for(char* ptr = look_group(groups[j]); ptr; ptr = get_rest())
                    puts(ptr);
                puts("");
            }

            puts("\nList combinations");
            for(combination combo = list_combinations(); combo.reagent1; combo = next_combination())
                printf("%s + %s = %s\n", combo.reagent1, combo.reagent2, combo.result);

            puts("\nCheck combinations");
            for(combination combo = list_combinations(); combo.reagent1; combo = next_combination()){
                char* ptr = check_combination(combo.reagent1, combo.reagent2);
                printf("Original: %s + %s = %s\n", combo.reagent1, combo.reagent2, ptr ? ptr : "<<Null>>");
            }

            //for(char* ptr = partial_match_elements("g"); ptr; ptr = get_rest())
            //    puts(ptr);
        }
            
        load("test");

        {
            puts("\nElement list:");
            for(char* ptr = list_elements(); ptr; ptr = get_rest())
                puts(ptr);

            puts("\nGroup list:");
            size_t i = 0;
            for(char* ptr = list_groups(); ptr; ptr = get_rest()){
                puts(ptr);
                i++;
            }

            char* groups[i];
            i = 0;
            for(char* ptr = list_groups(); ptr; ptr = get_rest())
                groups[i++] = ptr;

            puts("\nLook groups:");
            for(int j = 0; j < i; j++){
                puts(groups[j]);
                for(char* ptr = look_group(groups[j]); ptr; ptr = get_rest())
                    puts(ptr);
                puts("");
            }

            puts("\nList combinations");
            for(combination combo = list_combinations(); combo.reagent1; combo = next_combination())
                printf("%s + %s = %s\n", combo.reagent1, combo.reagent2, combo.result);

            puts("\nCheck combinations");
            for(combination combo = list_combinations(); combo.reagent1; combo = next_combination()){
                char* ptr = check_combination(combo.reagent1, combo.reagent2);
                printf("Original: %s + %s = %s\n", combo.reagent1, combo.reagent2, ptr ? ptr : "<<Null>>");
            }

            //for(char* ptr = partial_match_elements("g"); ptr; ptr = get_rest())
            //    puts(ptr);
        }
    }
}