#include "class.h"

__attribute__((minsize)) void AEFile::Write(char value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    char local = value;
    Write(1, &local, handle);

    uint32_t guardDelta = (uint32_t)cookie - (uint32_t)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}
