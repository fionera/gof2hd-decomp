#include "class.h"

uint32_t AEFile::Read(float &value, uint32_t handle)
{
    return Read(4, &value, handle);
}
