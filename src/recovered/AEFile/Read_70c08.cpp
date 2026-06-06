#include "class.h"

uint32_t AEFile::Read(int16_t &value, uint32_t handle)
{
    return Read(2, &value, handle);
}
