#include "class.h"

uint32_t AEFile::OpenWrite(String &path, uint32_t *handle) {
    return Open(path, OPEN_WRITE, handle);
}
