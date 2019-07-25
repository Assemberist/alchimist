#ifndef EFFECT_H
#define EFFECT_H

enum effect_t{
	FIRE,
	WET,
	SHOCK,
	SLASH,
	BEET,
	SHOOT
};

struct effect{
	size_t e_time;
	size_t e_val;
	effect_t e_type;
};

#endif
