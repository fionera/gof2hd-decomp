#include "class.h"

extern "C" void ShaderBaseStruct_ctor(ShaderBaseStruct *self);
extern "C" char BloomShader_vtable;
extern "C" void *BloomShader_typeinfo_source;
extern "C" void *BloomShader_typeinfo_dest;

namespace AbyssEngine {

__attribute__((minsize)) BloomShader::BloomShader()
{
    void *volatile cookie = __stack_chk_guard;
    ShaderBaseStruct_ctor((ShaderBaseStruct *)this);
    *(void **)this = &BloomShader_vtable + 8;
    *(void **)BloomShader_typeinfo_dest = *(void **)BloomShader_typeinfo_source;
    String name("BloomShader", false);
    *(String *)((char *)this + 0xc) = name;
    name.~String();
    unsigned int guardDelta = (unsigned int)__stack_chk_guard - (unsigned int)cookie;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
