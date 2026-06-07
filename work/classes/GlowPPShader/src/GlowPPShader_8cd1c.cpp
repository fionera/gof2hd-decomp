#include "class.h"

extern "C" void *GlowPPShader_vtable;
extern "C" void *GlowPPShader_typeinfo_source;
extern "C" void *GlowPPShader_typeinfo_dest;

extern "C" GlowPPShader *_ZN11AbyssEngine12GlowPPShaderC1Ev(GlowPPShader *self)
{
    void *name[3];
    name[0] = self;
    void * volatile cookie = __stack_chk_guard;
    ShaderBaseStruct_ctor((ShaderBaseStruct *)self);
    void **source = (void **)GlowPPShader_typeinfo_source;
    void **dest = (void **)GlowPPShader_typeinfo_dest;
    *(void **)self = (char *)GlowPPShader_vtable + 8;
    *dest = *source;
    String_ctor_char((String *)name, "GlowPPShader", false);
    String_assign((String *)((char *)self + 0xc), (String *)name);
    String_dtor((String *)name);
    uint32_t diff = (uint32_t)cookie;
    diff = (uint32_t)__stack_chk_guard - diff;
    if (diff == 0) {
        return self;
    }
    __stack_chk_fail(diff);
}
