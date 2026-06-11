#ifndef GOF2_COLORMIXADD_H
#define GOF2_COLORMIXADD_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

extern "C" void glUseProgram(uint32_t program);
extern "C" int glGetUniformLocation(uint32_t program, const char *name);
extern "C" int glGetAttribLocation(uint32_t program, const char *name);
extern "C" void glUniform1f(int location, float value);
extern "C" void glUniform1i(int location, int value);
extern "C" void glUniform3f(int location, float x, float y, float z);
extern "C" void glUniform3fv(int location, int count, const float *value);
extern "C" void glUniform4fv(int location, int count, const float *value);
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



extern "C" char _ZTVN11AbyssEngine11ColorMixAddE[];





class ColorMixAdd : public ShaderBaseStruct  {
public:
    static int ShaderIndex;

    int      program;                 // +0x4   GL program handle
    uint8_t  dirty;                 // +0x9   dirty flag
    String   name;                 // +0xc   shader name
    int      attribA0;                // +0x20  attrib a0
    int      attribA1;                // +0x24  attrib a1
    int      uniformU1;                // +0x28  uniform u1
    int      uniformU2;                // +0x2c  uniform u2
    int      uniformU0;                // +0x30  uniform u0
    int      uniformU4;                // +0x34  uniform u4
    int      uniformU3;                // +0x38  uniform u3
    int      uniformU5;                // +0x3c  uniform u5

    ColorMixAdd();
    void Init(Engine *);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine
#endif
