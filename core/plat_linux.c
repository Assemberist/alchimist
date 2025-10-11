#include "platform.h"

#ifdef __linux__

bool linux_dir_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

#endif