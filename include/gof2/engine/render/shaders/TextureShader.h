#ifndef GOF2_TEXTURESHADER_H
#define GOF2_TEXTURESHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)

extern "C" uint8_t g_TextureShader_hasSecondProgram;
extern "C" uint8_t g_TextureShader_activeSlot;

extern "C" void *g_TextureShader_typeInfoSource;
extern "C" void *g_TextureShader_vtable;
extern "C" void *g_TextureShader_typeInfoTarget;

extern "C" int glGetUniformLocation(uint32_t program, const char *name);
extern "C" int glGetAttribLocation(uint32_t program, const char *name);
extern "C" void glUseProgram(uint32_t program);
extern "C" void glUniform1i(int location, int value);
extern "C" void glUniform1f(int location, float value);
extern "C" void glUniform3f(int location, float x, float y, float z);
extern "C" void glUniform3fv(int location, int count, const float *value);
extern "C" void glUniform4fv(int location, int count, const float *value);
extern "C" void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glEnableVertexAttribArray(uint32_t index);
extern "C" void glDisableVertexAttribArray(uint32_t index);
extern "C" void glVertexAttribPointer(uint32_t index, int size, uint32_t type, uint8_t normalized, int stride,
                                      const void *pointer);
extern "C" void glBindBuffer(uint32_t target, uint32_t buffer);

void operator delete(void *ptr) noexcept;

namespace AbyssEngine {

struct Engine;
struct Mesh;

namespace AEMath {
struct Vector;
}

extern "C" float *Vector_to_float(AEMath::Vector *self);

// String_ctor_char / String_assign / String_dtor are declared in gof2/String.h.

// AbyssEngine::TextureShader — GLES2 textured-mesh shader. Holds two shader
// programs (base + extended); per-slot uniform/attribute locations are stored as
// 2-element arrays indexed by the active slot (slot*4 byte stride == one int).
class TextureShader {
public:
    void *vtable;       // vtable
    uint32_t program;    // program slot 0
    uint8_t field_0x8;
    uint8_t dirty;     // dirty flag
    uint8_t field_0xa;
    uint8_t field_0xb;
    String name;      // name
    uint32_t programExt;   // program slot 1
    int positionAttrib[2];     // position attrib
    int texcoordAttrib[2];     // texcoord attrib
    int mvpUniform[2];     // mvp uniform
    int colorUniform[2];     // color uniform
    int textureUniform[2];     // texture uniform
    int worldViewUniform[2];     // worldView uniform
    int textureModeUniform[2];     // textureMode uniform
    int lightUniform[2];     // light uniform
    int fogNearUniform[2];     // fogNear uniform
    int fogFarUniform[2];     // fogFar uniform
    int activeTextureUniform[2];     // activeTexture uniform
    int fogColorUniform[2];     // fogColor uniform
    int alphaUniform[2];     // alpha uniform

    void Init(Engine *engine);
    void UseShader(bool flag);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void ConnectShaderComponents(uint32_t program, int slot);
};

} // namespace AbyssEngine

#endif
