#include "class.h"

uint32_t AEFile::Read(uint8_t &value, uint32_t handle)
{
    return Read(1, &value, handle);
}
