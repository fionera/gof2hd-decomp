#ifndef GOF2_BUMPSHADERREFRACT_H
#define GOF2_BUMPSHADERREFRACT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" char _ZTVN11AbyssEngine12BumpShaderRefractE[];

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

} // namespace AbyssEngine

struct BumpShaderRefract {
    String field_0xc;                   // +0xc
};
#endif
