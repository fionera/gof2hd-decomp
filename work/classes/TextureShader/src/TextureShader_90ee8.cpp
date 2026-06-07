#include "class.h"

extern "C" TextureShader *TextureShader_TextureShader(TextureShader *self)
{
    struct Frame {
        void *nameStorage[3];
        void *cookie;
    } frame;

    frame.nameStorage[0] = self;
    frame.cookie = __stack_chk_guard;

    ShaderBaseStruct_ctor(self);

    void **source = (void **)g_TextureShader_typeInfoSource;
    void **target = (void **)g_TextureShader_typeInfoTarget;
    *(void *volatile *)self = (char *)g_TextureShader_vtable + 8;
    *target = *source;

    String_ctor_char((String *)frame.nameStorage, "TextureShader", false);
    String_assign((String *)(bytes(self) + 0x0c), (String *)frame.nameStorage);
    String_dtor((String *)frame.nameStorage);

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)frame.cookie;
    if (guardDelta == 0) {
        return self;
    }
    __stack_chk_fail(guardDelta);
}
