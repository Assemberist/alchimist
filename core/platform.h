#pragma once

#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
    #define dir_exists windows_dir_exists

    #include <direct.h>
    #define create_dir(path) _mkdir(path)

#elif defined(__linux__)

    #include <sys/stat.h>
    #define dir_exists linux_dir_exists

    #include <sys/types.h>
    #define create_dir(path) mkdir(path, 0755)

#endif

bool dir_exists(const char* path);
