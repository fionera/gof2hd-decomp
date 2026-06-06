#include "class.h"

uint32_t AEFile::OpenRead(String &path, uint32_t *handle) {
    return Open(path, OPEN_READ, handle);
}
