%{

#include <dirent.h>
#include "parser.h"

extern void yyrestart(FILE*);
extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char *str){ fprintf(stderr,"ошибка: %s\n",str);}
int yywrap(){ return 1; }



phase Phase;

typedef struct {
	uint32_t openFlag: 1;
	uint32_t groupNamePos: 11;
	uint32_t namePos: 20;
} element_t;

typedef struct {
	uint16_t id1;
	uint16_t id2;
	uint16_t idResult;
} combination_t;

// Prepare

char* get_combi_path(char* path);
char* get_group_path(char* path);

// Phase 1

FILE* elem_txt;

size_t elem_counter = 0;
size_t elem_name_ptr = 0;

size_t group_name_ptr = 0;

FILE* binary;

void new_element(int is_open, char* el);

// Phase 2

token* wordbook;

// Phase 3

size_t combination_counter = 0;
combination_t* combiArray;

int cmp(const void* val1, const void* val2);
void new_combo(size_t reagent1, size_t reagent2, size_t result);

// Tools

#ifdef _WIN32
	#define cat_tool "test"
	#define del_tool "del"

#elif defined __linux__
	#define cat_tool "cat"
	#define del_tool "rm"

#else
	#error "Custom error: Not supported OS"

#endif

////////////////////////////////
//
// Main
//
////////////////////////////////

int main(int argc, char* argv[]) {
	char* group_path;
	char* combi_path;

	switch(argc){
		case 2:
			combi_path = get_combi_path(argv[1]);
			break;

		case 3:
			combi_path = argv[2];
			break;

		default:
			perror("Usage: parse <folder with groups> [<combinations.txt>]");
			return 1;
	}

	group_path = get_group_path(argv[1]);

	////////////////////////////////
	//
	// Phase 1: parse groups
	//
	// <group>/*.txt -> groups.txt		|| group names
	// 				 -> elements.txt	|| element names
	//				 -> binary.dat		|| element data
	//
	////////////////////////////////

	Phase = PHASE1;

    DIR* dir;
    struct dirent* ent;
    if (dir = opendir(group_path)) {
		FILE* groups = fopen("groups.txt", "w");
		elem_txt = fopen("elements.txt", "w");
		binary = fopen("binary.dat", "w");

		char buffer[256];
        while (ent = readdir(dir)) {
            if (ent->d_name[0] == '.')
				continue;

			if (strstr(ent->d_name, ".txt")){
				sprintf(buffer, "%s/%s", group_path, ent->d_name);

				FILE* reader = fopen(buffer, "r");
                if(reader){
					yyrestart(reader);
					yyparse();
					fclose(reader);

					// get file name w/o ext.
					strcpy(buffer, ent->d_name);
					strtok(buffer, ".");

					fprintf(groups, "%s", buffer);
					putc('\0', groups);
					group_name_ptr += strlen(buffer)+1;
				}
				else fprintf(stderr, "Can't open %s\n", ent->d_name);
            }
        }

		fclose(groups);
		fclose(elem_txt);
		fclose(binary);

		free(group_path);
        closedir(dir);

    } else {
        perror("could not open directory");
        return 1;
    }

	////////////////////////////////
	//
	// Phase 2: make string_tree
	//
	// 	elements.txt -> string_tree
	//
	//  string_tree is map #{ char* => size_t }
	//
	////////////////////////////////

	Phase = PHASE2;

	wordbook = init_tree();

	FILE* words = fopen("elements.txt", "r");
	if(words){
		yyrestart(words);
		yyparse();
		fclose(words);
	} else {
        perror("can't open elemnts (phase 2)");
        return 1;
    }


	////////////////////////////////
	//
	// Phase 3: parse combinations
	//
	// combinations.txt ->
	//	   string_tree ->
	//		   append binary.dat
	//
	////////////////////////////////

	Phase = PHASE3;

	FILE *combinations = fopen(combi_path, "r");

    if (!combinations) {
        perror("can't open combinations");
        return 1;
    }

    char buf[4096];

    while (fgets(buf, sizeof(buf), combinations))
        for (char *p = buf; *p; p++)
            if (*p == '=')
				combination_counter++;

	rewind(combinations);

	combination_t combo[combination_counter];
	combiArray = combo;
	combination_counter = 0;

	yyrestart(combinations);
	yyparse();
	fclose(combinations);

	qsort(combo, combination_counter, sizeof(combination_t), cmp);

	if(binary = fopen("binary.dat", "a")){
		fwrite(combo, sizeof(combination_t), combination_counter, binary);
	} else {
        perror("can't open bin data");
        return 1;
    }

	////////////////////////////////
	//
	// Phase 4: concat data to archive
	//
	// metadata
	// 		2b: amount of elements 
	//		2b: amount of combinations
	//		4b: start of group names
	//		4b: start of element names
	// elements
	// combinations
	// wordbook
	// group names
	// element names
	//
	////////////////////////////////

	FILE* output = fopen("library.alch2", "w");

	if(output){
		uint16_t val16 = elem_counter;
		fwrite(&val16, sizeof(uint16_t), 1, output);
		val16 = combination_counter;
		fwrite(&val16, sizeof(uint16_t), 1, output);

		// pack tree and check how much space it takes
		uint32_t val32 = group_name_ptr;
		fwrite(&val32, sizeof(uint32_t), 1, output);
		val32 = elem_name_ptr;
		fwrite(&val32, sizeof(uint32_t), 1, output);


		sprintf(buf, "%s binary.dat >> library.alch2", cat_tool);
		remove("binary.dat");

		// write wordbook

		sprintf(buf, "%s groups.txt >> library.alch2", cat_tool);
		remove("groups.txt");
		sprintf(buf, "%s elements.txt >> library.alch2", cat_tool);
		remove("elements.txt");

	} else {
        perror("can't open bin data");
        return 1;
    }

	system("");

	////////////////////////////////
	//
	// Cleanup
	//
	////////////////////////////////

	if(argc == 3)
		free(combi_path);
}

void new_element(int is_open, char* el){
	element_t elem = { is_open, group_name_ptr, elem_name_ptr };
	fwrite(&elem, sizeof(element_t), 1, binary);
	elem_counter++;

	fprintf(elem_txt, "%s", el);
	putc('\0', elem_txt);
	elem_name_ptr += strlen(el) + 1;
}

int cmp(const void* val1, const void* val2){
	return *((uint32_t*)val1) - *((uint32_t*)val2);
}

void new_combo(size_t reagent1, size_t reagent2, size_t result){
	if(reagent1 > reagent2){
		size_t tmp = reagent1;
		reagent1 = reagent2;
		reagent2 = tmp;
	}

	combiArray[combination_counter].id1 = reagent1;
	combiArray[combination_counter].id2 = reagent2;
	combiArray[combination_counter].idResult = result;

	combination_counter++;
}

char* get_combi_path(char* path){
    char* ptr = (char*)malloc(strlen(path)+strlen("/combinations.txt")+1);
    sprintf(ptr, "%s%s", path, "/combinations.txt");
    return ptr;
}

char* get_group_path(char* path){
    char* ptr = (char*)malloc(strlen(path)+strlen("/groups")+1);
    sprintf(ptr, "%s%s", path, "/groups");
    puts(ptr);
    return ptr;
}

%}

////////////////////////////////
//
// Bison part
//
////////////////////////////////

%union {
	char* str;
	size_t id;
}

%token SPACE OPEN PLUS EQ END
%token <str> WORD_P1
%token WORD_P2
%token <id> ID

%%

parse: phase1
	 | phase2
	 | phase3



phase1: if_spaces elements if_spaces END

elements: element
		| elements SPACE element

element: WORD_P1 { new_element(0, $1); }
	   | OPEN if_spaces WORD_P1 { new_element(1, $3); }



phase2: words END

words: WORD_P2
	 | words WORD_P2



phase3: if_spaces combinations if_spaces END

combinations: combination
			| combinations SPACE combination

combination: ID if_spaces PLUS if_spaces ID if_spaces EQ if_spaces ID { new_combo($1, $5, $9); }
		   | ID if_spaces EQ if_spaces ID if_spaces PLUS if_spaces ID { new_combo($9, $5, $1); }



if_spaces:
		 | SPACE

%%
