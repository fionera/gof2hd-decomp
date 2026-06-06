#include "class.h"

uint32_t AEFile::ReadSwitched(uint16_t &value, uint32_t handle) {
    uint32_t result = Read(2, &value, handle);
    if (result == 0) {
        return result;
    }
    value = __builtin_bswap16(value);
    return result;
}
