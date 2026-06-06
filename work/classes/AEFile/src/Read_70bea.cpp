#include "class.h"

uint32_t AEFile::Read(uint32_t &value, uint32_t handle)
{
    return Read(4, &value, handle);
}
