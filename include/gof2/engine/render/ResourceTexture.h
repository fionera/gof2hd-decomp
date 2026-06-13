#ifndef GOF2_RESOURCETEXTURE_H
#define GOF2_RESOURCETEXTURE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - AbyssEngine::ResourceTexture.
// Layout (from ctors):
//   +0x00 char*  name   (heap copy of a C-string)
//   +0x04 float  value  (param_2)
// Access fields via byte-offset casts from `this`.

// Defined at top level to match fwd.h's `struct ResourceTexture;` forward decl.
// (game-qualified name is AbyssEngine::ResourceTexture)
class ResourceTexture {
public:
    char *name;                    // +0x0  heap copy of name C-string
    float value;                    // +0x4  value

    ResourceTexture(const char *name, float value);
    ResourceTexture(const String &name, float value);
    ~ResourceTexture();
};

extern "C" {
void *operator_new__(uint32_t size);
void operator_delete__(void *ptr);
void __aeabi_memcpy(void *dst, const void *src, uint32_t n);
}
// String_GetStringLength / String_GetAEChar are declared canonically in gof2/String.h.
#endif
