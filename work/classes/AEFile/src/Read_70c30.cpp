#include "class.h"

uint32_t AEFile::Read(bool &value, uint32_t handle)
{
    return Read(1, &value, handle);
}
