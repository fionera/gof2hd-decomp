#ifndef GOF2_BUMPSHADERV4_H
#define GOF2_BUMPSHADERV4_H
#include "gof2/common.h"
// struct derived from offset-access field map (named typed members, // +0xNN comments)
#include <new>

extern "C" char _ZTVN11AbyssEngine12BumpShaderV4E[];

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

// Local copy of the shader base layout (vtable +0x0, program +0x4, flags +0x8). Kept
// byte-for-byte identical to AbyssEngine::ShaderBaseStruct so this TU stays link-compatible.
struct ShaderBaseStruct {
    static int shaderIndexIntern;

    void *field_0x0;                    // +0x0  vtable
    int field_0x4;                      // +0x4  GL program handle
    volatile uint16_t field_0x8;        // +0x8  flags

    ShaderBaseStruct();
    ~ShaderBaseStruct();

    int ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
};

// AbyssEngine::BumpShaderV4 — GLES2 bump-mapping shader variant (derives from ShaderBaseStruct).
// Members are the GL program handle, the shader name, the light-dirty flag and the
// attribute/uniform LOCATION handles (a0..a4, u0..u9) resolved in Init().
class BumpShaderV4 : public ShaderBaseStruct  {
public:
    uint8_t field_0x9;                  // +0x9  light-dirty flag
    String field_0xc;                   // +0xc  shader name
    int field_0x20;                     // +0x20 attrib a0 (position)
    int field_0x24;                     // +0x24 attrib a1 (texcoord)
    int field_0x28;                     // +0x28 attrib a2 (normal)
    int field_0x2c;                     // +0x2c attrib a3 (tangent)
    int field_0x30;                     // +0x30 attrib a4 (bitangent)
    int field_0x34;                     // +0x34 uniform u0 (mvp matrix)
    int field_0x38;                     // +0x38 uniform u1 (normal matrix)
    int field_0x3c;                     // +0x3c uniform u2 (light dir/pos)
    int field_0x40;                     // +0x40 uniform u3 (light color)
    int field_0x44;                     // +0x44 uniform u4 (sampler0)
    int field_0x48;                     // +0x48 uniform u5 (sampler1)
    int field_0x4c;                     // +0x4c uniform u6
    int field_0x50;                     // +0x50 uniform u7
    int field_0x54;                     // +0x54 uniform u8
    int field_0x58;                     // +0x58 uniform u9

    static int ShaderIndex;

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    BumpShaderV4();
};

} // namespace AbyssEngine

#endif
