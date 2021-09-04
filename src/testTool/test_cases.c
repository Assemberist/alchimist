#include "int_check.h"
#include <stdbool.h>

#define SUCCESS 0;
#define ERROR 1;

int missing(library* lib){
	int flag = SUCCESS;
	int i = lib->recept_count;
	while(i--){
		combinate* com = lib->recepts + i;
		char *names[3] = {
			com->reagent1 ? get_el_name(com->reagent1) : NULL,
			com->reagent2 ? get_el_name(com->reagent2) : NULL,
			com->rezult ? get_el_name(com->rezult) : NULL
		};
		if(names[0] && names[1] && names[2]) continue;

		flag = ERROR;
		printf(
			"Record corrupted: %s + %s = %s\n",
			names[0] ? names[0] : "???",
			names[1] ? names[1] : "???",
			names[2] ? names[2] : "???"
		);
	}

	return flag;
}

int again_elements(library* lib){
	int flag = SUCCESS;
	size_t i, j;
	for(i = lib->group_count; i--;){
		for(j = lib->groups[i].name_count; j--;){
			element* val = lib->groups[i].names + j;
			if(find_element(get_el_name(val), lib->worterbuch) != val){
				printf("Duplicate of element found: %s::%s\n", lib->groups[i].name, get_el_name(val));
				flag = ERROR;
			}
		}
	}

	return flag;
}

int again_combinate(library* lib){
	int flag = SUCCESS;
	size_t i, j;
	for(i = lib->recept_count; --i;){
		combinate* com_i = lib->recepts + i;
		for(j = i; j--;){
			combinate* com_j = lib->recepts + j;
			if(	(com_i->reagent1 == com_j->reagent1 && com_i->reagent2 == com_j->reagent2) ||
				(com_i->reagent1 == com_j->reagent2 && com_i->reagent2 == com_j->reagent1)
			){
				printf("Duplicat of recept found: %s::%s\n",
						get_el_name(com_i->reagent1),
						get_el_name(com_i->reagent2));

				flag = ERROR;
			}
		}
	}

	return flag;
}

int unused(library* lib){
	int flag = SUCCESS;
	size_t i, j;
	for(i = lib->recept_count; i--;)
		lib->recepts[i].rezult->shortName.is_open = true;

	for(i = lib->group_count; i--;){
		group* grp = lib->groups + i;
		for(j = grp->name_count; j--;){
			if(grp->names[j].shortName .is_open == NULL){
				printf("Element %s::%s is not participant of any recepts\n",
					grp->name,
					get_el_name(grp->names + j)
				);
				flag = ERROR;
			}
		}
	}

	return flag;
}

int undiscovered(library* lib){
	size_t replaced = 0;

	int replace_flag;
	for(replace_flag = true; replace_flag; replace_flag = false){
		size_t i = lib->recept_count - replaced;

		while(i--){
			combinate com = lib->recepts[i];
			if(com.reagent1->shortName.is_open && com.reagent2->shortName.is_open){
				replaced++;
				replace_flag = true;
				com.rezult->shortName.is_open = true;
				lib->recepts[i] = lib->recepts[lib->recept_count - replaced];
				lib->recepts[lib->recept_count - replaced] = com;
			}
		}

		if(replaced == lib->recept_count)
			return SUCCESS;
	}

	size_t i = lib->recept_count - replaced;
	while(i--){
		combinate* com = lib->recepts + i;
		printf("Recept %s+%s=%s is unreacheble\n",
			get_el_name(com->reagent1),
			get_el_name(com->reagent2),
			get_el_name(com->rezult)
		);
	}

	return ERROR;
}