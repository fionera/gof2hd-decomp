#ifndef GOF2_GREENSHADER_H
#define GOF2_GREENSHADER_H
#include "gof2/common.h"
#include <new>
// struct derived from offset-access field map (named typed members)

namespace AbyssEngine {

struct Engine;
struct Mesh;

// AbyssEngine::ShaderBaseStruct base layout used by GreenShader (truncated local copy;
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

// AbyssEngine::GreenShader — GLES2 green shader (derives from ShaderBaseStruct).
// Caches five vertex-attribute locations (a0..a4) and nine uniform locations (u0..u8)
// after Init resolves them from the linked program.
class GreenShader : public ShaderBaseStruct {
public:
    String name;            // +0xc  shader name

    int a0Loc;              // +0x20 attribute a0
    int a1Loc;              // +0x24 attribute a1
    int a2Loc;              // +0x28 attribute a2
    int a3Loc;              // +0x2c attribute a3
    int a4Loc;              // +0x30 attribute a4
    int u0Loc;              // +0x34 uniform u0
    int u1Loc;              // +0x38 uniform u1
    int u2Loc;              // +0x3c uniform u2
    int u3Loc;              // +0x40 uniform u3
    int u4Loc;              // +0x44 uniform u4
    int u5Loc;              // +0x48 uniform u5
    int u6Loc;              // +0x4c uniform u6
    int u7Loc;              // +0x50 uniform u7
    int u8Loc;              // +0x54 uniform u8

    GreenShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

extern "C" {
extern char GreenShader_vtable[];
extern const char GreenShader_name[];

int glGetAttribLocation(int program, const char *name);
int glGetUniformLocation(int program, const char *name);
void glUseProgram(uint32_t program);
void glUniform1i(int location, int value);
void glUniform1f(int location, float value);
void glUniform3f(int location, float x, float y, float z);
void glUniform3fv(int location, int count, const float *value);
void glUniform4fv(int location, int count, const float *value);
void glDisableVertexAttribArray(int index);
void glEnableVertexAttribArray(int index);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const float *value);
void glVertexAttribPointer(int index, int size, unsigned int type, uint8_t normalized,
                           int stride, const void *pointer);
void glBindBuffer(unsigned int target, unsigned int buffer);

void operator_delete(void *ptr) noexcept;
}

#endif
