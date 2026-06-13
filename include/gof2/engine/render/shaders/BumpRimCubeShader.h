#ifndef GOF2_BUMPRIMCUBESHADER_H
#define GOF2_BUMPRIMCUBESHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (named typed members)
#include <new>

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

// AbyssEngine::ShaderBaseStruct base layout used by BumpRimCubeShader (truncated local copy;
// only the leading trivial fields are modelled here so the placement-new in the ctor constructs
// just those; the non-trivial String name and the per-shader location members below are real
// derived members).
struct ShaderBaseStruct {
    void *field_0x0;                    // +0x0 vtable
    int field_0x4;                      // +0x4 GL program handle
    volatile uint16_t field_0x8;        // +0x8 flags

    static int shaderIndexIntern;

    ShaderBaseStruct();
    ~ShaderBaseStruct();

    int ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
    int LoadBindShader(const char *vertexPath, const char *fragmentPath);
};

// AbyssEngine::BumpRimCubeShader — GLES2 bump + rim + cubemap shader (derives from
// ShaderBaseStruct). Caches five vertex-attribute locations (a0..a4) and 26 uniform
// locations (u0..u25) after Init resolves them from the linked program.
class BumpRimCubeShader : public ShaderBaseStruct  {
public:
    uint8_t dirty;          // +0x9  per-frame uniform-upload gate (light-dirty flag)
    String name;            // +0xc  shader name

    int a0Loc;              // +0x20 attribute a0
    int a1Loc;              // +0x24 attribute a1
    int a2Loc;              // +0x28 attribute a2
    int a3Loc;              // +0x2c attribute a3
    int u0Loc;              // +0x30 uniform u0
    int u1Loc;              // +0x34 uniform u1
    int u2Loc;              // +0x38 uniform u2
    int u3Loc;              // +0x3c uniform u3
    int u4Loc;              // +0x40 uniform u4
    int u5Loc;              // +0x44 uniform u5
    int u6Loc;              // +0x48 uniform u6
    int u7Loc;              // +0x4c uniform u7
    int u8Loc;              // +0x50 uniform u8
    int u9Loc;              // +0x54 uniform u9
    int u10Loc;             // +0x58 uniform u10
    int u11Loc;             // +0x5c uniform u11
    int u14Loc;             // +0x60 uniform u14
    int u12Loc;             // +0x64 uniform u12
    int u15Loc;             // +0x68 uniform u15
    int u13Loc;             // +0x6c uniform u13
    int u16Loc;             // +0x70 uniform u16
    int u17Loc;             // +0x74 uniform u17
    int u18Loc;             // +0x78 uniform u18
    int u19Loc;             // +0x7c uniform u19
    int u20Loc;             // +0x80 uniform u20
    int u21Loc;             // +0x84 uniform u21
    int u22Loc;             // +0x88 uniform u22
    int u23Loc;             // +0x8c uniform u23
    int u24Loc;             // +0x90 uniform u24
    int u25Loc;             // +0x94 uniform u25

    static int ShaderIndex;

    BumpRimCubeShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
