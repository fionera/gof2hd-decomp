#include "class.h"

namespace AbyssEngine {

__attribute__((minsize)) SimpleShader::SimpleShader()
{
    void *volatile cookie = __stack_chk_guard;
    ShaderBaseStruct_ctor((ShaderBaseStruct *)this);
    *(String *)((char *)this + 0xc) = String("SimpleShader", false);
    *(void **)this = (char *)SimpleShader_vtable + 8;
    SimpleShader_ShaderIndex = ShaderBaseStruct_shaderIndexIntern;
    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
