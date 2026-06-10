#ifndef GOF2_TEXTURESHADER_H
#define GOF2_TEXTURESHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)

extern "C" uint8_t g_TextureShader_hasSecondProgram;
extern "C" uint8_t g_TextureShader_activeSlot;

extern "C" void *g_TextureShader_typeInfoSource;
extern "C" void *g_TextureShader_vtable;
extern "C" void *g_TextureShader_typeInfoTarget;

extern "C" void *ShaderBaseStruct_ctor(void *self);
extern "C" void *ShaderBaseStruct_dtor(void *self);
extern "C" uint32_t ShaderBaseStruct_ES2LoadProgram(void *self, const char *vertex, const char *fragment);

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

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(uint32_t diff);

void operator delete(void *ptr) noexcept;

namespace AbyssEngine {

struct Engine;
struct Mesh;

namespace AEMath {
struct Vector;
}

extern "C" float *Vector_to_float(AEMath::Vector *self);

extern "C" void String_ctor_char(String *self, const char *text, bool copy);
extern "C" String *String_assign(String *self, const String *other);
extern "C" void String_dtor(String *self);

// AbyssEngine::TextureShader — GLES2 textured-mesh shader. Holds two shader
// programs (base + extended); per-slot uniform/attribute locations are stored as
// 2-element arrays indexed by the active slot (slot*4 byte stride == one int).
struct TextureShader {
    void *field_0x0;       // vtable
    uint32_t field_0x4;    // program slot 0
    uint8_t field_0x8;
    uint8_t field_0x9;     // dirty flag
    uint8_t field_0xa;
    uint8_t field_0xb;
    String field_0xc;      // name
    uint32_t field_0x20;   // program slot 1
    int field_0x24[2];     // position attrib
    int field_0x2c[2];     // texcoord attrib
    int field_0x34[2];     // mvp uniform
    int field_0x3c[2];     // color uniform
    int field_0x44[2];     // texture uniform
    int field_0x4c[2];     // worldView uniform
    int field_0x54[2];     // textureMode uniform
    int field_0x5c[2];     // light uniform
    int field_0x64[2];     // fogNear uniform
    int field_0x6c[2];     // fogFar uniform
    int field_0x74[2];     // activeTexture uniform
    int field_0x7c[2];     // fogColor uniform
    int field_0x84[2];     // alpha uniform

    void Init(Engine *engine);
    void UseShader(bool flag);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void ConnectShaderComponents(uint32_t program, int slot);
};

} // namespace AbyssEngine

#endif
