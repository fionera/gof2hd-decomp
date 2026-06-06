#include "class.h"

uint32_t AEFile::Read(uint16_t &value, uint32_t handle)
{
    return Read(2, &value, handle);
}
