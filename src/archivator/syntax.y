%{

#include <dirent.h>
#include "string_tree.h"

void yyerror(const char *str){ fprintf(stderr,"ошибка: %s\n",str);}
int yywrap(){ return 1; }

typedef enum {
	PHASE1,
	PHASE2,
	PHASE3,
} phase;

phase Phase;
token* wordbook;

size_t elem_counter = 0;
size_t group_name_ptr = 0;

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
	//				 -> elements.dat	|| element data
	//
	////////////////////////////////
	
	Phase = PHASE1;
	
    DIR* dir;
    struct dirent* ent;
    if (dir = opendir(group_path)) {
		FILE* groups = fopen("groups.txt", "w");
		FILE* elem_txt = fopen("elements.txt", "w");
		FILE* elem_dat = fopen("elements.dat", "w");

        while (ent = readdir(dir)) {
            if (ent->d_name[0] == '.')
				continue;
			
			if (strstr(ent->d_name, ".txt"){

				FILE* reader = fopen(ent->d_name, "r");
                if(reader){
					yyrestart(reader);
					yyparse();
					fclose(reader);
					
					write_group(groups, ent->d_name);
				}
				else fprintf(stderr, "Can't open %s\n", ent->d_name);
            }
        }

		fclose(groups);
		fclose(elem_txt);
		fclose(elem_dat);
		
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
	// 	elements.txt | -> string_tree =
	//	elements.dat |		#{ char* => size_t }
	//	
	//
	////////////////////////////////
/*
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
	//		   combinations.dat
	//
	////////////////////////////////
	
	Phase = PHASE3;

	FILE* combinations = fopen(combi_path, "r");
	if(combinations){
		yyrestart(combinations);
		yyparse();
		fclose(combinations);
	} else {
        perror("can't open combinations");
        return 1;
    }

	free(combi_path);
	
	*/
}

%}

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

// groups part

phase1: if_spaces elements if_spaces END

elements: element
		| elements SPACE element

element: WORD_P1 { new_element($1); }
	   | OPEN if_spaces WORD_P1 { new_open_element($3); }

if_spaces:
		 | SPACE
		 
// create string_tree (local)

phase2: words END

words: WORD_P2
	 | words WORD_P2

// combinations part

phase3: if_spaces combinations if_spaces END

combinations: combination
			| combinations SPACE combination

combination: ID if_spaces PLUS if_spaces ID if_spaces EQ if_spaces ID { new_combo($1, $5, $9); }
		   | ID if_spaces EQ if_spaces ID if_spaces PLUS if_spaces ID { new_combo($9, $5, $1); }

%%