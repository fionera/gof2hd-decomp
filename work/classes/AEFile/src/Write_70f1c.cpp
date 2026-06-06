#include "class.h"

extern "C" void __stack_chk_fail(...) __attribute__((noreturn));

void AEFile::Write(int8_t value, uint32_t handle)
{
    char padding[3];
    int8_t local;
    void * volatile cookie = __stack_chk_guard;

    padding[0] = 0;
    local = value;
    Write(1, &local, handle);
    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}
