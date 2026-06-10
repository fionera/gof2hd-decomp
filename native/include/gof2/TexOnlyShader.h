#ifndef GOF2_TEXONLYSHADER_H
#define GOF2_TEXONLYSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(uint32_t diff);

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

extern "C" void String_ctor_char(String *self, const char *text, bool copy);
extern "C" void String_assign(String *self, const String *other);
extern "C" void String_dtor(String *self);

// AbyssEngine::TexOnlyShader — minimal GLES2 shader that only samples a texture.
struct TexOnlyShader {
    void *field_0x0;     // vtable
    uint32_t field_0x4;  // program handle
    uint8_t field_0x8;
    uint8_t field_0x9;   // dirty flag
    uint8_t field_0xa;
    uint8_t field_0xb;
    String field_0xc;    // name
    int field_0x20;      // a_position attrib
    int field_0x24;      // a_texCoord attrib
    int field_0x28;      // u_WorldMatrix uniform
    int field_0x2c;      // s_texture uniform

    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
    void Init(Engine *engine);
    TexOnlyShader();
};

} // namespace AbyssEngine

#endif
