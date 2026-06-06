#include "class.h"

uint32_t AEFile::Read(char &value, uint32_t handle) {
    return Read(1, &value, handle);
}
