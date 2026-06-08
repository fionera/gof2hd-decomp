#include "class.h"

extern "C" int ShaderBaseStruct_shaderIndexIntern;
extern "C" int BumpShaderV2_ShaderIndex;

namespace AbyssEngine {

// AbyssEngine::BumpShaderV2::BumpShaderV2()
BumpShaderV2::BumpShaderV2()
{
    void *volatile cookie = __stack_chk_guard;

    ShaderBaseStruct_ctor(this);

    // Install our vtable (points one slot past the RTTI/offset header).
    *(uint32_t *)this = (uint32_t)BumpShaderV2_vtable + 8;

    // Remember the global shader index assigned by the base ctor.
    BumpShaderV2_ShaderIndex = ShaderBaseStruct_shaderIndexIntern;

    // Name string lives at +0xc; build a temp and copy-assign it in.
    String tmp("BumpShaderV2", false);
    String_assign((String *)((char *)this + 0xc), &tmp);
    String_dtor(&tmp);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}

} // namespace AbyssEngine
