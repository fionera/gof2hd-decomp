#include "class.h"

uint32_t __attribute__((cold)) AEFile::ReadSwitched(int32_t &value, uint32_t handle)
{
    uint32_t result = Read(4, &value, handle);
    if (__builtin_unpredictable(result != 0)) {
        value = __builtin_bswap32(value);
    }
    return result;
}
