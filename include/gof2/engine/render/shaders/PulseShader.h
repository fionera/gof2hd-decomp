#ifndef GOF2_PULSESHADER_H
#define GOF2_PULSESHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (named typed members)
#include <new>

#include "gof2/engine/render/Mesh.h"   // full Mesh layout (top-level ::Mesh)

struct Engine;            // top-level, matches fwd.h (layout not modelled in this batch)

namespace AbyssEngine {

// AbyssEngine::ShaderBaseStruct base layout used by PulseShader (truncated local copy;
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

// AbyssEngine::PulseShader — GLES2 pulse shader (derives from ShaderBaseStruct).
// Caches five vertex-attribute locations (a0..a4) and ten uniform locations (u0..u9)
// after Init resolves them from the linked program.
class PulseShader : public ShaderBaseStruct {
public:
    uint8_t dirty;          // +0x9  per-frame uniform-upload gate
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
    int u5Loc;              // +0x44 uniform u5
    int u6Loc;              // +0x48 uniform u6
    int u7Loc;              // +0x4c uniform u7
    int u8Loc;              // +0x50 uniform u8
    int u9Loc;              // +0x54 uniform u9
    int u4Loc;              // +0x58 uniform u4

    PulseShader();
    void Init(::Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, ::Engine *engine);
};

} // namespace AbyssEngine

// Byte-offset accessors into the render Engine object (its full layout is not modelled
// in this batch -- the matrices / light vectors it carries are read by fixed offset).

extern "C" {
extern char PulseShader_vtable[];
extern const char PulseShader_name[];

int glGetAttribLocation(int program, const char *name);
int glGetUniformLocation(int program, const char *name);
void glUseProgram(uint32_t program);
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

void operator_delete(void *ptr) noexcept;
}

#endif
