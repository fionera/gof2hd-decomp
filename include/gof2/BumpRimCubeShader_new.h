#ifndef GOF2_BUMPRIMCUBESHADER_NEW_H
#define GOF2_BUMPRIMCUBESHADER_NEW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" char _ZTVN11AbyssEngine21BumpRimCubeShader_newE[];

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

    ShaderBaseStruct();
    ~ShaderBaseStruct();

    int ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
    int LoadBindShader(const char *vertexPath, const char *fragmentPath);
};



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

// BumpRimCubeShader_new — derives from ShaderBaseStruct (vtable at +0x0).
// Int fields are GL uniform/attribute locations (contiguous block 0x20..0x94).
struct BumpRimCubeShader_new {
    void *field_0x0;            // +0x0  vtable ptr
    int field_0x4;              // +0x4  program
    uint8_t pad_0x8;            // +0x8
    uint8_t field_0x9;          // +0x9  needsUpdate flag
    uint16_t pad_0xa;
    String field_0xc;           // +0xc  shader name
    int pad_0x1c;
    int field_0x20;  int field_0x24;  int field_0x28;  int field_0x2c;  int field_0x30; // attribs a0..a4
    int field_0x34;  int field_0x38;  int field_0x3c;  int field_0x40;  int field_0x44; // u0..u4
    int field_0x48;  int field_0x4c;  int field_0x50;  int field_0x54;  int field_0x58; // u5..u9
    int field_0x5c;  int field_0x60;  int field_0x64;  int field_0x68;  int field_0x6c; // u10..u14
    int field_0x70;  int field_0x74;  int field_0x78;  int field_0x7c;  int field_0x80; // u15..u19
    int field_0x84;  int field_0x88;  int field_0x8c;  int field_0x90;  int field_0x94; // u20..u24

    static int ShaderIndex;

    BumpRimCubeShader_new();
    void Init(Engine *);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine
#endif
