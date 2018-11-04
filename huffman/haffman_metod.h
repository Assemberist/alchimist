#ifndef HAFFMAN_H
#define HAFFMAN_H

#include <iostream>
#include <string>

using namespace std;

struct point{
	point* next;
	point* down0;
	point* down1;
	int count;
	char symbol; 

	point();
	~point();
};

void read_symbols(point* begin, char* path);
point* make_tree(point* begin);
void combinate(point* min0, point* min1);
void recursiv_output(point* begin, string str, int pos);
void haffman_table(const char* path);

#endif
