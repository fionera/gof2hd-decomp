#include "class.h"

using AbyssEngine::ShaderBaseStruct;
using AbyssEngine::String;
using AbyssEngine::shader_name;
using AbyssEngine::shader_paths;

extern "C" ShaderBaseStruct *ShaderBaseStruct_8e424(ShaderBaseStruct *self)
{
    register uint32_t *stackGuardPtr = &__stack_chk_guard;
    volatile uint32_t stackGuard = *stackGuardPtr;

    shader_vtable(self) = (uint32_t)ShaderBaseStruct_vtable + 8;
    new (shader_name(self)) String();
    *(volatile uint32_t *)((char *)self + 0x4) = 0xffffffff;
    *(volatile uint16_t *)((char *)self + 0x8) = 0x100;
    ++ShaderBaseStruct_count;
    shader_paths(self) = 0;

    char nameStorage[sizeof(String)];
    new ((String *)nameStorage) String("", false);
    *shader_name(self) = *(String *)nameStorage;
    ((String *)nameStorage)->~String();

    uint32_t stackDifference = *stackGuardPtr - stackGuard;
    if (stackDifference != 0) {
        __stack_chk_fail(stackDifference);
    }
    return self;
}
