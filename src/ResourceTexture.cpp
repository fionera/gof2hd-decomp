#include "gof2/ResourceTexture.h"
#include "gof2/String.h"




// ---- ResourceTexture_112c88.cpp ----
// AbyssEngine::ResourceTexture::ResourceTexture(char const*, float)
ResourceTexture::ResourceTexture(const char *name, float value)
{
    uint32_t len = String_GetStringLength(name);
    char *buf = (char *)operator_new__(len + 1U);
    this->name = buf;
    __aeabi_memcpy(buf, name, len + 1U);
    this->value = value;
}

// ---- _ResourceTexture_77ebc.cpp ----
// AbyssEngine::ResourceTexture::~ResourceTexture()
ResourceTexture::~ResourceTexture()
{
    operator_delete__(this->name);
    this->name = 0;
}

// ---- ResourceTexture_11b6d2.cpp ----
// AbyssEngine::ResourceTexture::ResourceTexture(AbyssEngine::String const&, float)
ResourceTexture::ResourceTexture(const String &name, float value)
{
    char *utf8 = (char *)((String *)(&name))->GetAEChar();
    uint32_t len = String_GetStringLength(utf8);
    char *buf = (char *)operator_new__(len + 1U);
    this->name = buf;
    __aeabi_memcpy(buf, utf8, len + 1U);
    this->value = value;
    operator_delete__(utf8);
}
