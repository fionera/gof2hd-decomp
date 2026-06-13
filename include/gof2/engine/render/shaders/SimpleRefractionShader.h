#ifndef GOF2_SIMPLEREFRACTIONSHADER_H
#define GOF2_SIMPLEREFRACTIONSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (named typed members)
#include <new>

namespace AbyssEngine {

struct Engine;
struct Mesh;

// AbyssEngine::ShaderBaseStruct base layout used by SimpleRefractionShader (truncated local
// copy; the canonical class lives in ShaderBaseStruct.h). Only the leading trivial fields are
// modelled here so the placement-new in the ctor constructs just those; the non-trivial
// String name and the per-shader location members below are real derived members.
struct ShaderBaseStruct {
    void *field_0x0;                    // +0x0 vtable
    int field_0x4;                      // +0x4 GL program handle (-1 when unset)
    volatile uint16_t field_0x8;        // +0x8 flags

    static int shaderIndexIntern;

    ShaderBaseStruct();
    ~ShaderBaseStruct();
    uint32_t ES2LoadProgram(const char *vertexSource, const char *fragmentSource);
};

// AbyssEngine::SimpleRefractionShader — GLES2 refraction shader (derives from ShaderBaseStruct).
// Caches four vertex-attribute locations and the uniform locations resolved by Init from the
// linked program.
class SimpleRefractionShader : public ShaderBaseStruct {
public:
    static int ShaderIndex;

    uint8_t dirty;          // +0x9  per-frame uniform-upload gate
    String name;            // +0xc  shader name

    int aPositionLoc;       // +0x20 attribute a_position
    int aNormalLoc;         // +0x24 attribute a_normal
    int aTangentLoc;        // +0x28 attribute a_tangent
    int aTexCoordLoc;       // +0x2c attribute a_texCoord
    int uM0Loc;             // +0x30 uniform u_m0
    int uM1Loc;             // +0x34 uniform u_m1
    int uTex0Loc;           // +0x38 uniform u_tex0
    int uTex1Loc;           // +0x3c uniform u_tex1
    int uM4Loc;             // +0x40 uniform u_m4
    int uM3Loc;             // +0x44 uniform u_m3
    int uM2Loc;             // +0x48 uniform u_m2
    int uRefractLoc;        // +0x4c uniform u_refract
    int uM5Loc;             // +0x50 uniform u_m5
    int uM6Loc;             // +0x54 uniform u_m6

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

extern "C" {
extern char _ZTVN11AbyssEngine22SimpleRefractionShaderE[];
extern const char SimpleRefractionShader_name[];

void glUseProgram(uint32_t program);
int glGetUniformLocation(uint32_t program, const char *name);
int glGetAttribLocation(uint32_t program, const char *name);
void glUniform1i(int location, int value);
void glUniform1f(int location, float value);
void glUniform3f(int location, float x, float y, float z);
void glUniform3fv(int location, int count, const float *value);
void glUniform4fv(int location, int count, const float *value);
void glUniformMatrix3fv(int location, int count, uint8_t transpose, const void *value);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
void glEnableVertexAttribArray(uint32_t index);
void glDisableVertexAttribArray(uint32_t index);
void glBindBuffer(uint32_t target, uint32_t buffer);
void glActiveTexture(uint32_t texture);
void glBindTexture(uint32_t target, uint32_t texture);
void glVertexAttribPointer(
    uint32_t index, int size, uint32_t type, uint8_t normalized, int stride, const void *pointer);
}

#endif
