#ifndef GOF2_RESOURCETEXTURE_H
#define GOF2_RESOURCETEXTURE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - AbyssEngine::ResourceTexture.
// Layout (from ctors):
//   +0x00 char*  name   (heap copy of a C-string)
//   +0x04 float  value  (param_2)
// Access fields via byte-offset casts from `this`.


namespace AbyssEngine {





} // namespace AbyssEngine

using String = AbyssEngine::String;
using ResourceTexture = AbyssEngine::ResourceTexture;

extern "C" {
void *operator_new__(uint32_t size);
void operator_delete__(void *ptr);
void __aeabi_memcpy(void *dst, const void *src, uint32_t n);

// AbyssEngine::String::GetStringLength(char const*) -- strlen.
uint32_t String_GetStringLength(const char *s);
// AbyssEngine::String::GetAEChar() const -- returns a freshly allocated UTF-8 C-string.
void *String_GetAEChar(const String *self);
}

struct ResourceTexture {
    float field_0x4;                    // +0x4
};
#endif
