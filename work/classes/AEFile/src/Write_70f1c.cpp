#include "class.h"

__attribute__((minsize)) void AEFile::Write(int8_t value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    int8_t local = value;
    Write(1, &local, handle);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)cookie - (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}
