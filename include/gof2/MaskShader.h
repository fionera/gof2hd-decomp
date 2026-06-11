#ifndef GOF2_MASKSHADER_H
#define GOF2_MASKSHADER_H
#include "gof2/common.h"
// Galaxy on Fire 2 - AbyssEngine::MaskShader (GLES2 masked/textured shader).
// Derives from ShaderBaseStruct. Field names use the deterministic field_0xNN convention.
// Engine is a global-scope type (::Engine, defined in gof2/Engine.h).
struct Engine;

namespace AbyssEngine {

struct Mesh;

// AbyssEngine::ShaderBaseStruct base layout used by MaskShader.
struct ShaderBaseStruct {
    void *field_0x0;                    // +0x0 vtable
    int field_0x4;                      // +0x4 GL program handle
    volatile uint16_t field_0x8;        // +0x8
};

// AbyssEngine::MaskShader
class MaskShader : public ShaderBaseStruct  {
public:
    uint8_t field_0x9;                  // +0x9 color-dirty flag
    String field_0xc;                   // +0xc shader name
    int field_0x20;                     // +0x20 attribute a_position
    int field_0x24;                     // +0x24 attribute a_texCoord
    int field_0x28;                     // +0x28 attribute a_color
    int field_0x2c;                     // +0x2c uniform u_mvpMatrix
    int field_0x30;                     // +0x30 uniform u_texture0
    int field_0x34;                     // +0x34 uniform u_texture1
    int field_0x38;                     // +0x38 uniform u_color

    static int ShaderIndex;

    MaskShader();
    void Init(::Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, ::Engine *engine);
};

} // namespace AbyssEngine


extern "C" {
void ShaderBaseStruct_ctor(void *self);
void *ShaderBaseStruct_dtor(void *self);
uint32_t ShaderBaseStruct_ES2LoadProgram(void *self, const char *vertex, const char *fragment);
void Engine_SetTextureSlot(void *self, uint32_t a, uint32_t b);

int glGetAttribLocation(uint32_t program, const char *name);
int glGetUniformLocation(uint32_t program, const char *name);
void glUseProgram(uint32_t program);
void glUniform1i(int location, int value);
void glUniform4fv(int location, int count, const float *value);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
void glEnableVertexAttribArray(uint32_t index);
void glDisableVertexAttribArray(uint32_t index);
void glVertexAttribPointer(uint32_t index, int size, uint32_t type, uint8_t normalized, int stride, const void *pointer);
void glBindBuffer(uint32_t target, uint32_t buffer);

extern void *MaskShader_vtable[];
extern int32_t ShaderBaseStruct_shaderIndexIntern;

void operator_delete(void *ptr) noexcept;
}

#endif
