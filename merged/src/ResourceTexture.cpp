#include "ResourceTexture.h"




// ---- ResourceTexture_112c88.cpp ----
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

// ---- _ResourceTexture_77ebc.cpp ----
namespace AbyssEngine {

// AbyssEngine::ResourceTexture::~ResourceTexture()
ResourceTexture::~ResourceTexture()
{
    operator_delete__(*(void **)this);
    *(void **)this = 0;
}

} // namespace AbyssEngine

// ---- ResourceTexture_11b6d2.cpp ----
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
