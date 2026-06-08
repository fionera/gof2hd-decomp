#include "class.h"

namespace AbyssEngine {

// AbyssEngine::ResourceTexture::ResourceTexture(AbyssEngine::String const&, float)
ResourceTexture::ResourceTexture(const String &name, float value)
{
    void *utf8 = String_GetAEChar(&name);
    uint32_t len = *(uint32_t *)((char *)&name + 0x8);
    void *buf = operator_new__(len + 1U);
    *(void **)this = buf;
    __aeabi_memcpy(buf, utf8, len + 1U);
    *(float *)((char *)this + 0x4) = value;
    operator_delete__(utf8);
}

} // namespace AbyssEngine
