#ifndef GOF2_TEXONLYSHADER_H
#define GOF2_TEXONLYSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

struct Engine;
struct Mesh;
struct ShaderBaseStruct;





} // namespace AbyssEngine

using TexOnlyShader = AbyssEngine::TexOnlyShader;
using Engine = AbyssEngine::Engine;
using Mesh = AbyssEngine::Mesh;
using ShaderBaseStruct = AbyssEngine::ShaderBaseStruct;
using String = AbyssEngine::String;

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

void operator delete(void *ptr) noexcept;

extern "C" void *ShaderBaseStruct_ctor(ShaderBaseStruct *self);
extern "C" void *ShaderBaseStruct_dtor(ShaderBaseStruct *self);

extern "C" uint32_t g_TexOnlyShader_vtable[];
extern "C" int32_t g_ShaderBaseStruct_shaderIndexIntern;
extern "C" int32_t g_TexOnlyShader_shaderIndex;

extern "C" void String_ctor_char(String *self, const char *text, bool copy);
extern "C" void String_assign(String *self, const String *other);
extern "C" void String_dtor(String *self);
extern "C" uint32_t ShaderBaseStruct_ES2LoadProgram(ShaderBaseStruct *self, const char *vertex, const char *fragment);

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

template <class T>
static inline T &field(void *self, uint32_t offset)
{
    return *(T *)((char *)self + offset);
}

struct TexOnlyShader { void* _opaque; };  // no offset accesses observed
#endif
