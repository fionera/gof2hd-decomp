#ifndef GOF2_BUMPSHADER_H
#define GOF2_BUMPSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (named typed members)
#include <new>

namespace AbyssEngine {

struct Engine;
struct Mesh;

// AbyssEngine::ShaderBaseStruct base layout used by BumpShader (truncated local copy;
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

// AbyssEngine::BumpShader — GLES2 bump-mapping shader (derives from ShaderBaseStruct).
// Caches three vertex-attribute locations (a0..a2) and 22 uniform locations (u0..u21)
// after Init resolves them from the linked program.
class BumpShader : public ShaderBaseStruct {
public:
    uint8_t dirty;          // +0x9  per-frame uniform-upload gate
    String name;            // +0xc  shader name

    int a0Loc;              // +0x20 attribute a0 (position)
    int a1Loc;              // +0x24 attribute a1 (texcoord)
    int a2Loc;              // +0x28 attribute a2 (normal)
    int u0Loc;              // +0x2c uniform u0
    int u1Loc;              // +0x30 uniform u1
    int u2Loc;              // +0x34 uniform u2
    int u3Loc;              // +0x38 uniform u3
    int u4Loc;              // +0x3c uniform u4
    int u5Loc;              // +0x40 uniform u5
    int u6Loc;              // +0x44 uniform u6
    int u7Loc;              // +0x48 uniform u7
    int u8Loc;              // +0x4c uniform u8
    int u11Loc;             // +0x50 uniform u11
    int u9Loc;              // +0x54 uniform u9
    int u12Loc;             // +0x58 uniform u12
    int u10Loc;             // +0x5c uniform u10
    int u13Loc;             // +0x60 uniform u13
    int u14Loc;             // +0x64 uniform u14
    int u15Loc;             // +0x68 uniform u15
    int u16Loc;             // +0x6c uniform u16
    int u17Loc;             // +0x70 uniform u17
    int u18Loc;             // +0x74 uniform u18
    int u19Loc;             // +0x78 uniform u19
    int u20Loc;             // +0x7c uniform u20
    int u21Loc;             // +0x80 uniform u21

    BumpShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);

    // Bind a sampler uniform to a texture unit (thin glUniform1i wrapper).
    static void setSampler(int location, int unit);
};

} // namespace AbyssEngine

extern "C" {
extern char BumpShader_vtable[];
extern const char BumpShader_name[];

int glGetAttribLocation(int program, const char *name);
int glGetUniformLocation(int program, const char *name);
extern "C" void glUseProgram(uint32_t program);
void glUniform1i(int location, int value);
void glUniform1f(int location, float value);
void glUniform3f(int location, float x, float y, float z);
void glUniform4f(int location, float x, float y, float z, float w);
void glUniform3fv(int location, int count, const float *value);
void glUniform4fv(int location, int count, const float *value);
void glUniformMatrix3fv(int location, int count, uint8_t transpose, const float *value);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const float *value);
void glEnableVertexAttribArray(int index);
void glDisableVertexAttribArray(int index);
void glVertexAttribPointer(int index, int size, unsigned int type, uint8_t normalized,
                           int stride, const void *pointer);
void glBindBuffer(unsigned int target, unsigned int buffer);

float *Vector_cast_to_float(Vector *self);

void operator_delete(void *ptr) noexcept;
}

#endif
