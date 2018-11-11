#ifndef __STR_DIFFERENCIAL_H__
#define __STR_DIFFERENCIAL_H__

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void init_lexems();
void push_new_lexems(int a, string s[]);

int count_union(string &s, int &num);
int find_lexem(string &s, int n);

string start_differentiation(string input);
string summ_line(string &s, int &n);
string mul_line(string &s, int &n);
string proizvodnaya(string &s, int &n);

#endif
