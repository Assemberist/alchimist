#ifndef LOADER_H
#define LOADER_H

#include "library.h"
#include <dirent.h>
#include <unistd.h>

#ifdef __linux__
#include <linux/limits.h>

#elifdef _WIN32

#endif

#include <string.h>

library load_library(char* path);

#endif
