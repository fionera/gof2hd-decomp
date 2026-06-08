#include "class.h"

namespace AbyssEngine {

// AbyssEngine::ResourceTexture::ResourceTexture(char const*, float)
ResourceTexture::ResourceTexture(const char *name, float value)
{
    uint32_t len = String_GetStringLength(name);
    void *buf = operator_new__(len + 1U);
    *(void **)this = buf;
    __aeabi_memcpy(buf, name, len + 1U);
    *(float *)((char *)this + 0x4) = value;
}

} // namespace AbyssEngine
