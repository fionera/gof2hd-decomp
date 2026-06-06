#include "class.h"

uint32_t AEFile::OpenAppend(String &path, uint32_t *handle) {
    return Open(path, OPEN_APPEND, handle);
}
