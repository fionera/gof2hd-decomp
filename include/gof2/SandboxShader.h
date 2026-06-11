#ifndef GOF2_SANDBOXSHADER_H
#define GOF2_SANDBOXSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>

extern "C" char _ZTVN11AbyssEngine13SandboxShaderE[];

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



class SandboxShader : public ShaderBaseStruct  {
public:
    int      program;                  // +0x4   GL program handle
    uint8_t  dirty;                  // +0x9   dirty flag
    String   name;                  // +0xc   shader name
    int      attrPosition;                 // +0x20  attrib a_position
    int      attrNormal;                 // +0x24  attrib a_normal
    int      attrTangent;                 // +0x28  attrib a_tangent
    int      attrBinormal;                 // +0x2c  attrib a_binormal
    int      attrTexCoord;                 // +0x30  attrib a_texCoord
    int      uniformA;                 // +0x34  uniform u_a
    int      uniformB;                 // +0x38  uniform u_b
    int      uniformC;                 // +0x3c  uniform u_c
    int      uniformF;                 // +0x40  uniform u_f
    int      uniformD;                 // +0x44  uniform u_d
    int      uniformE;                 // +0x48  uniform u_e
    int      uniformG;                 // +0x4c  uniform u_g

    SandboxShader();
    void Init(Engine *);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine
#endif
