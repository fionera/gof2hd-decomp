#ifndef GOF2_RESOURCEMESH_H
#define GOF2_RESOURCEMESH_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - AbyssEngine::ResourceMesh.
// Layout (from ctor):
//   +0x00 char*          name   (heap copy of a C-string)
//   +0x04 unsigned short id     (param_2)
//   +0x06 bool           flag   (param_3)
// Access fields via byte-offset casts from `this`.


namespace AbyssEngine {



} // namespace AbyssEngine

using ResourceMesh = AbyssEngine::ResourceMesh;

extern "C" {
void *operator_new__(uint32_t size);
void operator_delete__(void *ptr);
void __aeabi_memcpy(void *dst, const void *src, uint32_t n);

// AbyssEngine::String::GetStringLength(char const*) -- strlen.
uint32_t String_GetStringLength(const char *s);
}

struct ResourceMesh {
    uint16_t field_0x4;                 // +0x4
    uint8_t field_0x6;                  // +0x6
};
#endif
