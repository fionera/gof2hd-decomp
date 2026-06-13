#ifndef GOF2_TEXTUREALPHATESTSHADER_H
#define GOF2_TEXTUREALPHATESTSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (named typed members)
#include <new>

void operator delete(void *ptr) noexcept;

using Vector = AbyssEngine::AEMath::Vector;

struct Engine;
struct Mesh;

namespace AbyssEngine {

// AbyssEngine::ShaderBaseStruct base layout used by TextureAlphaTestShader (truncated local
// copy; the canonical class lives in ShaderBaseStruct.h). Only the leading trivial fields are
// modelled here so the placement-new in the ctor constructs just those; the non-trivial String
// name and the per-program location members below are real derived members.
struct ShaderBaseStruct {
    void *field_0x0;                    // +0x0 vtable
    int field_0x4;                      // +0x4 GL program handle (opaque program)
    volatile uint16_t field_0x8;        // +0x8 flags

    static int shaderIndexIntern;

    ShaderBaseStruct();
    ~ShaderBaseStruct();
    uint32_t ES2LoadProgram(const char *vertexSource, const char *fragmentSource);
};

} // namespace AbyssEngine

// AbyssEngine::TextureAlphaTestShader — GLES2 texture shader with an optional alpha-test variant.
// Owns two GL programs: the opaque program (handle in field_0x4, index 0) and the alpha program
// (alphaProgram, index 1). Every cached attribute/uniform location is a 2-element array indexed
// by program (index 0 = opaque, index 1 = alpha), so location[i] lives at base+i*4 just like the
// original this+index*4 byte arithmetic.
class TextureAlphaTestShader : public AbyssEngine::ShaderBaseStruct  {
public:
    uint8_t dirty;              // +0x9  per-frame uniform-upload gate
    String name;               // +0xc  shader name

    int alphaProgram;          // +0x20 GL program handle (alpha-test variant, index 1)

    int aPositionLoc[2];       // +0x24 a_Position
    int aTexCoordLoc[2];       // +0x2c a_TexCoord
    int uMVPMatrixLoc[2];      // +0x34 u_MVPMatrix
    int uColorLoc[2];          // +0x3c u_Color
    int uTextureLoc[2];        // +0x44 u_Texture
    int uLightPosLoc[2];       // +0x4c u_LightPos
    int uAmbientLoc[2];        // +0x54 u_Ambient
    int uDiffuseLoc[2];        // +0x5c u_Diffuse
    int uSamplerLoc[2];        // +0x64 u_Sampler
    int uFogColorLoc[2];       // +0x6c u_FogColor

    TextureAlphaTestShader();
    ~TextureAlphaTestShader();
    void Init(Engine *);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void ConnectShaderComponents(int program, int index);
    void UseShader(bool);
};

extern "C" {
extern uint8_t g_TextureAlphaTestShader_useAlphaProgram;
extern uint8_t g_TextureAlphaTestShader_programIndex;
extern char g_TextureAlphaTestShader_vtable[];
extern int g_TextureAlphaTestShader_staticSource;
extern int g_TextureAlphaTestShader_staticDest;

int glGetUniformLocation(int program, const char *name);
int glGetAttribLocation(int program, const char *name);
void glUseProgram(int program);
void glDisableVertexAttribArray(int index);
void glEnableVertexAttribArray(int index);
void glUniform1f(int location, float value);
void glUniform1i(int location, int value);
void glUniform3f(int location, float x, float y, float z);
void glUniform3fv(int location, int count, const float *value);
void glUniform4fv(int location, int count, const float *value);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const float *value);
void glVertexAttribPointer(int index, int size, unsigned int type, uint8_t normalized,
                           int stride, const void *pointer);
void glBindBuffer(unsigned int target, unsigned int buffer);

float *Vector_cast_to_float(Vector *self);
void operator_delete(void *ptr) noexcept;
}

#endif
