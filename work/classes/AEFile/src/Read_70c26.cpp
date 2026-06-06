#include "class.h"

uint32_t AEFile::Read(int64_t &value, uint32_t handle)
{
    return AEFile::Read(8, &value, handle);
}
