#ifndef GOF2_DECOMP_VERTEX_COLOR_SHADER_CLASS_H
#define GOF2_DECOMP_VERTEX_COLOR_SHADER_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace AbyssEngine {

struct Engine;
struct Mesh;
struct ShaderBaseStruct;

struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;

    String(const char *text, bool copy);
    ~String();

    String &operator=(const String &other);
};

struct VertexColorShader {
    VertexColorShader();
    ~VertexColorShader();

    void SetInActive();
    void Init(Engine *engine);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
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

void operator delete(void *ptr) noexcept;
inline void *operator new(uint32_t, void *ptr) noexcept { return ptr; }

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

#endif
