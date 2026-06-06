#include "class.h"

extern "C" void __stack_chk_fail(...) __attribute__((noreturn));

void AEFile::Write(uint32_t value, uint32_t handle)
{
    struct Locals {
        uint32_t value;
        void * volatile cookie;
    };
    Locals locals;
    void *cookie = __stack_chk_guard;

    locals.value = value;
    locals.cookie = cookie;
    Write(4, &locals.value, handle);
    if ((uint32_t)__stack_chk_guard - (uint32_t)locals.cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
