#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "effect.h"
#include <string>

using std::vector;
using std::string;

class object{
protected:
	vector<effect> effects;
	string name;
public:
	void set_name(char* _name);
	void set_name(string str);
};

#endif
