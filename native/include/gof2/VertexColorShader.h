#ifndef GOF2_VERTEXCOLORSHADER_H
#define GOF2_VERTEXCOLORSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

struct Engine;
struct Mesh;
struct ShaderBaseStruct;





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

void operator delete(void *ptr) noexcept;
inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

struct VertexColorShader {
    String field_0xc;                   // +0xc
};
#endif
