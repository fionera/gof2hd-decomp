#include "class.h"

__attribute__((minsize)) uint32_t AEFile::ReadSwitched(uint16_t &value, uint32_t handle) {
    uint32_t result = Read(2, &value, handle);
    if (result != 0) {
        uint32_t v = value;
        value = (uint16_t)((v << 0x18) >> 0x10) | (v >> 8);
    }
    return result;
}
