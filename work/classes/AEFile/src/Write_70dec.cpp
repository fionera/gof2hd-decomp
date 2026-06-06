#include "class.h"

void AEFile::Write(char value, uint32_t handle)
{
    char local = value;
    void * volatile guard = __stack_chk_guard;
    Write(1, &local, handle);
    if (__stack_chk_guard == guard) {
        return;
    }
    __stack_chk_fail();
}
