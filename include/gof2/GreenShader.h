#ifndef GOF2_GREENSHADER_H
#define GOF2_GREENSHADER_H
#include "gof2/common.h"
#include <new>
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

struct Engine;
struct Mesh;

// AbyssEngine::ShaderBaseStruct base layout used by GreenShader.
struct ShaderBaseStruct;

} // namespace AbyssEngine

// NOTE: the original recovered header asserted sizeof(String)==0xc for the byte-matching
// 32-bit layout. This build uses the natural 64-bit std::u16string-backed String (sizeof 24),
// so that assertion is intentionally dropped.

// Per-class backing storage accessors. GreenShader's fields live at fixed byte offsets:
// GL program handle at 0x4, name String at 0xc, attribute locations 0x20..0x30 and
// uniform locations 0x34..0x54. They are read/written through these helpers.
static inline int &i32(void *self, uint32_t offset)
{
    return *(int *)((char *)self + offset);
}

static inline uint8_t &u8(void *self, uint32_t offset)
{
    return *(uint8_t *)((char *)self + offset);
}

extern "C" {
extern void *__stack_chk_guard;
extern char GreenShader_vtable[];
extern int GreenShader_typeInfoSource;
extern int GreenShader_typeInfoDest;
extern const char GreenShader_name[];

int glGetAttribLocation(int program, const char *name);
int glGetUniformLocation(int program, const char *name);
void glUseProgram(int program);
void glUniform1i(int location, int value);
void glUniform1f(int location, float value);
void glUniform3f(int location, float x, float y, float z);
void glUniform3fv(int location, int count, const float *value);
void glUniform4fv(int location, int count, const float *value);
void glDisableVertexAttribArray(int index);
void glEnableVertexAttribArray(int index);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const float *value);
void glVertexAttribPointer(int index, int size, unsigned int type, uint8_t normalized,
                           int stride, const void *pointer);
void glBindBuffer(unsigned int target, unsigned int buffer);

void ShaderBaseStruct_ctor(void *self);
void *ShaderBaseStruct_dtor(void *self) noexcept;
int ShaderBaseStruct_ES2LoadProgram(void *self, const char *vertexSource, const char *fragmentSource);

void operator_delete(void *ptr) noexcept;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;
}

namespace AbyssEngine {

// AbyssEngine::GreenShader — GLES2 green shader (derives from ShaderBaseStruct).
struct GreenShader {
    GreenShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);

    // raw field storage (offsets referenced through the i32/u8 helpers).
    // +0x0 vtable, +0x4 program handle, +0xc name String, +0x20..0x54 attrib/uniform locations.
    char field_storage_pre[0xc];        // +0x0
    String field_0xc;                   // +0xc shader name
    char field_storage_post[0x58 - 0x18]; // +0x18 .. 0x58
};

} // namespace AbyssEngine

#endif
