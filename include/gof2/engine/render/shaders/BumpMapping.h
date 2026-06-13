#ifndef GOF2_BUMPMAPPING_H
#define GOF2_BUMPMAPPING_H
#include "gof2/common.h"
#include <new>
// struct derived from offset-access field map (named typed members)

extern "C" char _ZTVN11AbyssEngine11BumpMappingE[];

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

// AbyssEngine::ShaderBaseStruct base layout used by BumpMapping (truncated local copy;
// the canonical class lives in ShaderBaseStruct.h). Only the leading trivial fields are
// modelled here so the placement-new in the ctor constructs just those; the non-trivial
// String name and the per-shader location members below are real derived members.
struct ShaderBaseStruct {
    void *field_0x0;                    // +0x0 vtable
    int field_0x4;                      // +0x4 GL program handle
    volatile uint16_t field_0x8;        // +0x8 flags

    static int shaderIndexIntern;

    ShaderBaseStruct();
    ~ShaderBaseStruct();
    uint32_t ES2LoadProgram(const char *vertexSource, const char *fragmentSource);
};

// AbyssEngine::BumpMapping — GLES2 bump-mapping shader (derives from ShaderBaseStruct).
// Caches five vertex-attribute locations (a0..a4) and four uniform locations (u0..u3)
// after Init resolves them from the linked program.
class BumpMapping : public ShaderBaseStruct {
public:
    uint8_t dirty;          // +0x9  per-frame uniform-upload gate
    String name;            // +0xc  shader name

    int a0Loc;              // +0x20 attribute a0 (a_position)
    int a1Loc;              // +0x24 attribute a1 (a_normal)
    int a2Loc;              // +0x28 attribute a2 (a_tangent)
    int a3Loc;              // +0x2c attribute a3 (a_binormal)
    int a4Loc;              // +0x30 attribute a4 (a_texCoord)
    int u0Loc;              // +0x34 uniform u0 (u_mvpMatrix)
    int u1Loc;              // +0x38 uniform u1 (u_lightDir)
    int u2Loc;              // +0x3c uniform u2 (u_texture)
    int u3Loc;              // +0x40 uniform u3 (u_normalMap)

    static int ShaderIndex;

    BumpMapping();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

// cross-class field accessors (Engine/Mesh are not in this batch; opaque here)

} // namespace AbyssEngine

#endif
