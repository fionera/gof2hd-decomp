#include "class.h"

void AEFile::Write(bool value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    Write(1, &value, handle);
    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}
