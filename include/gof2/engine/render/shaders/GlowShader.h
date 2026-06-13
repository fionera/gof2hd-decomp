#ifndef GOF2_GLOWSHADER_H
#define GOF2_GLOWSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (named typed members)
#include <new>

namespace AbyssEngine {

struct Engine;
struct Mesh;

// AbyssEngine::ShaderBaseStruct base layout used by GlowShader (truncated local copy;
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

// AbyssEngine::GlowShader — GLES2 glow shader (derives from ShaderBaseStruct).
// Caches two vertex-attribute locations (a_position/a_texCoord) and three uniform
// locations (u_mvp/u_color/u_texture) after Init resolves them from the linked program.
class GlowShader : public ShaderBaseStruct {
public:
    static int ShaderIndex;

    String name;                // +0xc  shader name

    int a_positionLoc;          // +0x20 attribute a_position
    int a_texCoordLoc;          // +0x24 attribute a_texCoord
    int u_mvpLoc;               // +0x28 uniform u_mvp
    int u_colorLoc;             // +0x2c uniform u_color
    int u_textureLoc;           // +0x30 uniform u_texture

    GlowShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

extern "C" char _ZTVN11AbyssEngine10GlowShaderE[];

extern "C" {
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
