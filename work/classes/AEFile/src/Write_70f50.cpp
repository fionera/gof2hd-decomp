#include "class.h"

extern "C" void __stack_chk_fail(...) __attribute__((noreturn));

void AEFile::Write(int64_t value, uint32_t handle)
{
    struct Locals {
        uint32_t low;
        uint32_t high;
        uint32_t unused;
        uint32_t guard;
    };
    volatile Locals locals;
    uint32_t guard = reinterpret_cast<uint32_t>(__stack_chk_guard);

    locals.high = static_cast<uint32_t>(static_cast<uint64_t>(value) >> 32);
    locals.low = static_cast<uint32_t>(value);
    locals.guard = guard;
    Write(8, const_cast<uint32_t *>(&locals.low), handle);
    if (locals.guard == reinterpret_cast<uint32_t>(__stack_chk_guard)) {
        return;
    }
    __stack_chk_fail();
}
