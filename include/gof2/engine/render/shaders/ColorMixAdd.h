#ifndef GOF2_COLORMIXADD_H
#define GOF2_COLORMIXADD_H
#include "gof2/common.h"
// struct derived from offset-access field map (named typed members)
#include <new>

namespace AbyssEngine {

struct Engine;
struct Mesh;

// AbyssEngine::ShaderBaseStruct base layout used by ColorMixAdd (truncated local copy;
// the canonical class lives in ShaderBaseStruct.h). Only the leading trivial fields are
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

// AbyssEngine::ColorMixAdd — GLES2 color-mix/add shader (derives from ShaderBaseStruct).
// Caches two vertex-attribute locations (a0,a1) and six uniform locations (u0..u5)
// after Init resolves them from the linked program.
class ColorMixAdd : public ShaderBaseStruct {
public:
    static int ShaderIndex;

    uint8_t dirty;          // +0x9  per-frame uniform-upload gate
    String name;            // +0xc  shader name

    int a0Loc;              // +0x20 attribute a0
    int a1Loc;              // +0x24 attribute a1
    int u1Loc;              // +0x28 uniform u1
    int u2Loc;              // +0x2c uniform u2
    int u0Loc;              // +0x30 uniform u0
    int u4Loc;              // +0x34 uniform u4
    int u3Loc;              // +0x38 uniform u3
    int u5Loc;              // +0x3c uniform u5

    ColorMixAdd();
    void Init(Engine *engine);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine

extern "C" char _ZTVN11AbyssEngine11ColorMixAddE[];

extern "C" {
void glUseProgram(uint32_t program);
int glGetUniformLocation(uint32_t program, const char *name);
int glGetAttribLocation(uint32_t program, const char *name);
void glUniform1f(int location, float value);
void glUniform1i(int location, int value);
void glUniform3f(int location, float x, float y, float z);
void glUniform3fv(int location, int count, const float *value);
void glUniform4fv(int location, int count, const float *value);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
void glEnableVertexAttribArray(uint32_t index);
void glDisableVertexAttribArray(uint32_t index);
void glBindBuffer(uint32_t target, uint32_t buffer);
void glVertexAttribPointer(
    uint32_t index, int size, uint32_t type, uint8_t normalized, int stride, const void *pointer);
}

#endif
