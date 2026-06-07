#include "class.h"

namespace AbyssEngine {

TexOnlyShader::TexOnlyShader()
{
    void **guard = &__stack_chk_guard;
    void *volatile cookie = *guard;
    ShaderBaseStruct_ctor((ShaderBaseStruct *)this);
    field<uint32_t>(this, 0x0) = (uint32_t)g_TexOnlyShader_vtable + 8;
    g_TexOnlyShader_shaderIndex = g_ShaderBaseStruct_shaderIndexIntern;
    String tmp;
    String_ctor_char(&tmp, "TexOnlyShader", false);
    String_assign((String *)((char *)this + 0xc), &tmp);
    String_dtor(&tmp);

    uint32_t guardDelta = (uint32_t)*guard - (uint32_t)cookie;
    if (guardDelta != 0) {
        __stack_chk_fail(guardDelta);
    }
}

} // namespace AbyssEngine
