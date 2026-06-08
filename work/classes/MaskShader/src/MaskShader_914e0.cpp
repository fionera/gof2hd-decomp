#include "class.h"

extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(
    AbyssEngine::ShaderBaseStruct *self);

namespace AbyssEngine {

int MaskShader::ShaderIndex;

// AbyssEngine::MaskShader::MaskShader()
MaskShader::MaskShader()
{
    void *volatile cookie = __stack_chk_guard;

    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);
    *(uint32_t *)this = (uint32_t)_ZTVN11AbyssEngine10MaskShaderE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    {
        String tmp("MaskShader", false);
        *(String *)((char *)this + 0xc) = tmp;
    }

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta != 0)
        __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
