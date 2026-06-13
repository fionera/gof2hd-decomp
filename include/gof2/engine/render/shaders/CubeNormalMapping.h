#ifndef GOF2_CUBENORMALMAPPING_H
#define GOF2_CUBENORMALMAPPING_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>

extern "C" char _ZTVN11AbyssEngine17CubeNormalMappingE[];

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



class CubeNormalMapping : public ShaderBaseStruct  {
public:
    int      program;                  // +0x4   GL program handle
    uint8_t  dirty;                  // +0x9   dirty flag
    String   name;                  // +0xc   shader name
    int      attribA0;                 // +0x20  attrib a0
    int      attribA1;                 // +0x24  attrib a1
    int      attribA2;                 // +0x28  attrib a2
    int      attribA3;                 // +0x2c  attrib a3
    int      attribA4;                 // +0x30  attrib a4
    int      uniformU0;                 // +0x34  uniform u0
    int      uniformU1;                 // +0x38  uniform u1
    int      uniformU2;                 // +0x3c  uniform u2
    int      uniformU3;                 // +0x40  uniform u3
    int      uniformU5;                 // +0x44  uniform u5
    int      uniformU4;                 // +0x48  uniform u4
    int      uniformU6;                 // +0x4c  uniform u6
    int      uniformU7;                 // +0x50  uniform u7
    int      uniformU8;                 // +0x54  uniform u8
    int      uniformU9;                 // +0x58  uniform u9
    int      uniformU12;                 // +0x5c  uniform u12
    int      uniformU10;                 // +0x60  uniform u10
    int      uniformU11;                 // +0x64  uniform u11

    CubeNormalMapping();
    void Init(Engine *);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine
#endif
