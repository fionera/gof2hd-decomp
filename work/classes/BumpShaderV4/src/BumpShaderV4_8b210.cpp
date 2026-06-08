#include "class.h"

extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(
    AbyssEngine::ShaderBaseStruct *self);

namespace AbyssEngine {

int BumpShaderV4::ShaderIndex;

__attribute__((minsize)) BumpShaderV4::BumpShaderV4()
{
    void *volatile cookie = __stack_chk_guard;
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);
    *(uint32_t *)this = (uint32_t)_ZTVN11AbyssEngine12BumpShaderV4E + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    {
        String tmp("BumpShaderV4", false);
        *(String *)((char *)this + 0xc) = tmp;
    }
    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
