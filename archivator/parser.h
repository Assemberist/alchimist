#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "string_tree/pack.h"

#define UINT64MAX (uint64_t)0-(uint64_t)1

typedef enum {
    PHASE1,
    PHASE2,
    PHASE3
} phase;