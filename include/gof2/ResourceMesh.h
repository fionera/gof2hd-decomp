#ifndef GOF2_RESOURCEMESH_H
#define GOF2_RESOURCEMESH_H
#include "gof2/common.h"
// Galaxy on Fire 2 - AbyssEngine::ResourceMesh.
// Layout (from ctor):
//   +0x00 char*          name   (heap copy of a C-string)
//   +0x04 unsigned short id
//   +0x06 bool           flag
namespace AbyssEngine {

struct ResourceMesh {
    char *field_0x0;                    // +0x0 heap copy of name
    uint16_t field_0x4;                 // +0x4 id
    uint8_t field_0x6;                  // +0x6 flag

    ResourceMesh(const char *name, unsigned short id, bool flag);
    ~ResourceMesh();
};

} // namespace AbyssEngine

extern "C" {
void *operator_new__(uint32_t size);
void operator_delete__(void *ptr);
void __aeabi_memcpy(void *dst, const void *src, uint32_t n);

// AbyssEngine::String::GetStringLength(char const*) -- strlen.
uint32_t String_GetStringLength(const char *s);
}

#endif
