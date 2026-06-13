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
class VertexColorShader {
public:
    byte   _base[0xc];   // +0x00 ShaderBaseStruct base
    String name;    // +0x0c shader name
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


// GLES2 entry-point function-pointer types (the engine dispatches some GL calls through
// loaded function pointers, e.g. glGetAttribLocation_ptr / glBindBuffer_ptr).
typedef int  GetShaderLocation(unsigned int program, const char *name);
typedef void BindBuffer(unsigned int target, unsigned int buffer);
typedef void VertexAttribPointer(unsigned int index, int size, unsigned int type, bool normalized,
                                 int stride, const void *pointer);

// String helpers (String_ctor_char / String_assign / String_dtor) are declared
// canonically in gof2/String.h.
#endif
