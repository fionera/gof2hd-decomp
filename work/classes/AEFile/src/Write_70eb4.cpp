#include "class.h"

__attribute__((minsize)) void AEFile::Write(int16_t value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    int16_t local = value;
    Write(2, &local, handle);

    uint32_t guardDelta = (uint32_t)cookie - (uint32_t)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}
