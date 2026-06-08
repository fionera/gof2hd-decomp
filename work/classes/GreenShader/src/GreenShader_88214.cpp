#include "class.h"

namespace AbyssEngine {

GreenShader::GreenShader()
{
    void *volatile cookie = __stack_chk_guard;

    ShaderBaseStruct_ctor(this);
    this->f_0 = (int)GreenShader_vtable + 8;
    GreenShader_typeInfoDest = GreenShader_typeInfoSource;

    {
        String temp(GreenShader_name, false);
        *(String *)((char *)this + 0xc) = temp;
    }

    uint32_t delta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (delta != 0)
        __stack_chk_fail(delta);
}

} // namespace AbyssEngine
