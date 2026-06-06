#include "class.h"

void AEFile::Write(uint16_t value, uint32_t handle)
{
    uint16_t local = value;
    Write(2, &local, handle);
}
