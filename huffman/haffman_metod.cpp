#include "haffman_metod.h"

point::point(){
	count=1;
	symbol=0;
	down0=0;
	down1=0;
	next=0;
}

point::~point(){

}

// read symbols in file and creat list of 'point'
void read_symbols(point* begin, char* path){
	point* end = begin;
	FILE* file = fopen(path, "r");
	end->symbol = fgetc(file);

	char sym;
	
	while((sym = fgetc(file))!=EOF){
		while(end->next){
			if(sym==end->symbol){
				end->count++;
				goto next_sym;
			}
			end = end->next;
		}
		if(end->symbol!=sym){
			end->next = new point;
			end->next->symbol = sym;
		}
		else end->count++;
		next_sym: end = begin;
	}
	
	fclose(file);
}

// buld tree haffman. Return pointer to an top of tree
point* make_tree(point* begin){
	point* min0;
	point* min1 = new point;
	point* end=begin->next->next;
	min1->next=begin;
	begin=min1;
	while(end=begin->next->next){
		if(begin->next->count > begin->next->next->count){
			min1=begin;
			min0=begin->next;
		}
		else{
			min0=begin;
			min1=begin->next;
		}
		while(end->next){
			if(end->next->count < min1->next->count){
				if(end->next->count < min0->next->count) {
					min1=min0;
					min0=end;
				}
				else min1=end;
			}
			end=end->next;
		}
		combinate(min0, min1);
	}
	end=begin->next;
	delete begin;
	return end;
}

// Include new struct point and exclude structs with minimal count of symbols
void combinate(point* min0, point* min1){
	point* new_point=new point;
	new_point->count = min0->next->count + min1->next->count;
	new_point->down0 = min0->next;
	new_point->down1 = min1->next;
	if(min1->next==min0){
		min1->next=new_point;
		new_point->next=min0->next->next;
	}
	else{
		min1->next=min1->next->next;
		new_point->next=min0->next->next;
		min0->next = new_point;
	}
}

// output symbols and thier codes and clean memory
void recursiv_output(point* begin, string str, int pos){
	if(begin->down0){
		str.push_back('0');
		recursiv_output(begin->down0, str, pos+1);
		str.pop_back();
	}
	if(begin->down1){
		str.push_back('1');
		recursiv_output(begin->down1, str, pos+1);
		str.pop_back();
	}
	if(!(begin->down1 || begin->down0)) cout << begin->count << " symbol[" << begin->symbol << "] = " << str << '\n';
	delete begin;
}

void haffman_table(const char* path){
	point* begin = new point;
	read_symbols(begin, path);
	string str="";
	begin = make_tree(begin);
	recursiv_output(begin, str, 0);
	getchar();
}