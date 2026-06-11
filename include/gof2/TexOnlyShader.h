#ifndef GOF2_TEXONLYSHADER_H
#define GOF2_TEXONLYSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)


void operator delete(void *ptr) noexcept;

extern "C" void *ShaderBaseStruct_ctor(void *self);
extern "C" void *ShaderBaseStruct_dtor(void *self);
extern "C" uint32_t ShaderBaseStruct_ES2LoadProgram(void *self, const char *vertex, const char *fragment);

extern "C" uint32_t g_TexOnlyShader_vtable[];
extern "C" int32_t g_ShaderBaseStruct_shaderIndexIntern;
extern "C" int32_t g_TexOnlyShader_shaderIndex;

extern "C" int glGetAttribLocation(uint32_t program, const char *name);
extern "C" int glGetUniformLocation(uint32_t program, const char *name);
extern "C" void glUseProgram(uint32_t program);
extern "C" void glUniform1i(int location, int value);
extern "C" void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glEnableVertexAttribArray(uint32_t index);
extern "C" void glDisableVertexAttribArray(uint32_t index);
extern "C" void glVertexAttribPointer(uint32_t index, int size, uint32_t type, uint8_t normalized, int stride,
                                      const void *pointer);
extern "C" void glBindBuffer(uint32_t target, uint32_t buffer);

namespace AbyssEngine {

struct Engine;
struct Mesh;

// AbyssEngine::TexOnlyShader — minimal GLES2 shader that only samples a texture.
class TexOnlyShader {
public:
    void *field_0x0;     // vtable
    uint32_t program;  // program handle
    uint8_t field_0x8;
    uint8_t uniformsDirty;   // dirty flag
    uint8_t field_0xa;
    uint8_t field_0xb;
    String name;    // name
    int aPosition;      // a_position attrib
    int aTexCoord;      // a_texCoord attrib
    int uWorldMatrix;      // u_WorldMatrix uniform
    int sTexture;      // s_texture uniform

    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
    void Init(Engine *engine);
    TexOnlyShader();
};

} // namespace AbyssEngine

#endif
