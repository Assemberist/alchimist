#include "platform.h"

#ifdef _WIN32

bool windows_dir_exists(path){
    DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) return false; // Директория не существует
    if (attrs & FILE_ATTRIBUTE_DIRECTORY) return true;  // Директория существует
    return false;                                       // Это не директория
}

#endif