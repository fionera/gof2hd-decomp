#ifndef GOF2_BUMPRIMCUBESHADER_H
#define GOF2_BUMPRIMCUBESHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" char _ZTVN11AbyssEngine17BumpRimCubeShaderE[];

extern "C" void glUseProgram(uint32_t program);
extern "C" int glGetUniformLocation(uint32_t program, const char *name);
extern "C" int glGetAttribLocation(uint32_t program, const char *name);
extern "C" void glUniform1f(int location, float value);
extern "C" void glUniform1i(int location, int value);
extern "C" void glUniform3f(int location, float x, float y, float z);
extern "C" void glUniform3fv(int location, int count, const float *value);
extern "C" void glUniform4fv(int location, int count, const float *value);
extern "C" void glUniformMatrix3fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glEnableVertexAttribArray(uint32_t index);
extern "C" void glDisableVertexAttribArray(uint32_t index);
extern "C" void glBindBuffer(uint32_t target, uint32_t buffer);
extern "C" void glVertexAttribPointer(
    uint32_t index, int size, uint32_t type, uint8_t normalized, int stride, const void *pointer);

namespace AbyssEngine {

struct Engine;
struct Mesh;

struct ShaderBaseStruct {
    static int shaderIndexIntern;

    void *field_0x0;                    // +0x0 vtable
    int field_0x4;                      // +0x4 GL program handle
    volatile uint16_t field_0x8;        // +0x8 flags

    ShaderBaseStruct();
    ~ShaderBaseStruct();

    int ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
    int LoadBindShader(const char *vertexPath, const char *fragmentPath);
};

struct BumpRimCubeShader : ShaderBaseStruct {
    uint8_t field_0x9;                  // +0x9 light-dirty flag
    String field_0xc;                   // +0xc shader name
    int field_0x20;                     // +0x20 attrib a0
    int field_0x24;                     // +0x24 attrib a1
    int field_0x28;                     // +0x28 attrib a2
    int field_0x2c;                     // +0x2c attrib a3
    int field_0x30;                     // +0x30 uniform u0
    int field_0x34;                     // +0x34 uniform u1
    int field_0x38;                     // +0x38 uniform u2
    int field_0x3c;                     // +0x3c uniform u3
    int field_0x40;                     // +0x40 uniform u4
    int field_0x44;                     // +0x44 uniform u5
    int field_0x48;                     // +0x48 uniform u6
    int field_0x4c;                     // +0x4c uniform u7
    int field_0x50;                     // +0x50 uniform u8
    int field_0x54;                     // +0x54 uniform u9
    int field_0x58;                     // +0x58 uniform u10
    int field_0x5c;                     // +0x5c uniform u11
    int field_0x60;                     // +0x60 uniform u14
    int field_0x64;                     // +0x64 uniform u12
    int field_0x68;                     // +0x68 uniform u15
    int field_0x6c;                     // +0x6c uniform u13
    int field_0x70;                     // +0x70 uniform u16
    int field_0x74;                     // +0x74 uniform u17
    int field_0x78;                     // +0x78 uniform u18
    int field_0x7c;                     // +0x7c uniform u19
    int field_0x80;                     // +0x80 uniform u20
    int field_0x84;                     // +0x84 uniform u21
    int field_0x88;                     // +0x88 uniform u22
    int field_0x8c;                     // +0x8c uniform u23
    int field_0x90;                     // +0x90 uniform u24
    int field_0x94;                     // +0x94 uniform u25

    static int ShaderIndex;

    BumpRimCubeShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

// cross-class field accessors (Engine/Mesh are not in this batch; opaque here)
static inline int &field_i32(void *self, uint32_t offset)
{
    return *(int *)((char *)self + offset);
}

static inline uint8_t &field_u8(void *self, uint32_t offset)
{
    return *(uint8_t *)((char *)self + offset);
}

static inline float &field_f32(void *self, uint32_t offset)
{
    return *(float *)((char *)self + offset);
}

static inline void *field_ptr(void *self, uint32_t offset)
{
    return *(void **)((char *)self + offset);
}

} // namespace AbyssEngine

#endif
