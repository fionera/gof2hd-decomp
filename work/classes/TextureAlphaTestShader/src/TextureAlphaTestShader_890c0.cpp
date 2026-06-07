#include "class.h"

namespace AbyssEngine {

TextureAlphaTestShader::TextureAlphaTestShader()
{
    uint32_t storage[4];
    storage[3] = __stack_chk_guard;

    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    *(char *volatile *)this = g_TextureAlphaTestShader_vtable + 8;
    g_TextureAlphaTestShader_staticDest = g_TextureAlphaTestShader_staticSource;

    new ((String *)storage) String("TextureAlphaTestShader", false);
    string_at(this, 0x0c) = *(String *)storage;
    ((String *)storage)->~String();

    int diff = __stack_chk_guard - storage[3];
    if (diff == 0) {
        return;
    }
    __stack_chk_fail(diff);
}

} // namespace AbyssEngine
