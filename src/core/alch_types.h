#pragma once

#include <stdint.h>

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