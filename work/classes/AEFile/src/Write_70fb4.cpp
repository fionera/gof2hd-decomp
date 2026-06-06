#include "class.h"

extern "C" void __stack_chk_fail(...) __attribute__((noreturn));

void AEFile::Write(float value, uint32_t handle)
{
    uint32_t guard = (uint32_t)*(void * volatile *)&__stack_chk_guard;
    struct {
        float value;
        volatile uint32_t guard;
    } local;

    local.value = value;
    local.guard = guard;

    Write(4, &local.value, handle);

    if (((uint32_t)*(void * volatile *)&__stack_chk_guard - local.guard) == 0) {
        return;
    }
    __stack_chk_fail();
}
