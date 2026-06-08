#include "class.h"

extern "C" void ShaderBaseStruct_ctor(ShaderBaseStruct *self);
extern "C" void String_ctor_text(void *self, const char *text, bool copy);
extern "C" void String_assign(void *self, void *other);
extern "C" void String_dtor(void *self);
extern "C" char BlurShader_vtable[];
extern "C" void *BlurShader_typeinfo_source[];
extern "C" void *BlurShader_typeinfo_dest[];

extern "C" BlurShader *BlurShader_BlurShader(BlurShader *self)
{
    void *volatile cookie = __stack_chk_guard;
    void *name[3];
    ShaderBaseStruct_ctor((ShaderBaseStruct *)self);
    void **source = BlurShader_typeinfo_source;
    void **dest = BlurShader_typeinfo_dest;
    *(void **)self = BlurShader_vtable + 8;
    *dest = *source;
    String_ctor_text(name, "BlurShader", false);
    String_assign((char *)self + 0xc, name);
    String_dtor(name);
    *(uint32_t *)((char *)self + 0x58) = 0x92006800;
    *(uint32_t *)((char *)self + 0x5c) = 0x40000000;
    uint32_t guardDelta =
        stack_guard_delta(*(volatile uint32_t *)&cookie, (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard);
    if (guardDelta == 0) {
        return self;
    }
    __stack_chk_fail(guardDelta);
}
