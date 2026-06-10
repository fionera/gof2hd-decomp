#ifndef GOF2_VERTEXCOLORSHADER_H
#define GOF2_VERTEXCOLORSHADER_H
#include "gof2/common.h"
#include <new>
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

struct Engine;
struct Mesh;
struct ShaderBaseStruct;

// VertexColorShader: ES2 shader wrapper. Embeds a String name at +0xc; the
// preceding 0xc bytes are the ShaderBaseStruct base (vtable + program id + flags).
struct VertexColorShader {
    byte   _base[0xc];   // +0x00 ShaderBaseStruct base
    String field_0xc;    // +0x0c shader name
    // remaining shader-location ints (+0x20..+0x58) reached via ae_i32/ae_u32 macros
    VertexColorShader();
    ~VertexColorShader();
};


} // namespace AbyssEngine

inline int &ae_i32(void *self, uint32_t offset)
{
    return *(int *)((char *)self + offset);
}

inline uint32_t &ae_u32(void *self, uint32_t offset)
{
    return *(uint32_t *)((char *)self + offset);
}

inline uint8_t &ae_u8(void *self, uint32_t offset)
{
    return *(uint8_t *)((char *)self + offset);
}

inline void *&ae_ptr(void *self, uint32_t offset)
{
    return *(void **)((char *)self + offset);
}

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
#endif
