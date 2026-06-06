#include "class.h"

void AEFile::Write(int16_t value, uint32_t handle)
{
    int16_t local = value;
    Write(2, &local, handle);
}
