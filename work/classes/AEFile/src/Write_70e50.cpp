#include "class.h"

extern "C" void __stack_chk_fail(...) __attribute__((noreturn));

__attribute__((optsize)) void AEFile::Write(int32_t value, uint32_t handle)
{
    struct Locals {
        int32_t value;
        void *cookie;
    };
    volatile Locals locals;
    void *cookie = __stack_chk_guard;

    locals.value = value;
    locals.cookie = cookie;
    Write(4, const_cast<int32_t *>(&locals.value), handle);
    if (locals.cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}
