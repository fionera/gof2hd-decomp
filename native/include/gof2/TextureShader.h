#ifndef GOF2_TEXTURESHADER_H
#define GOF2_TEXTURESHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

struct Engine;
struct Mesh;
struct ShaderBaseStruct;





namespace AEMath {
struct Vector;
}

} // namespace AbyssEngine

using Engine = AbyssEngine::Engine;
using Mesh = AbyssEngine::Mesh;
using ShaderBaseStruct = AbyssEngine::ShaderBaseStruct;
using String = AbyssEngine::String;
using TextureShader = AbyssEngine::TextureShader;
using Vector = AbyssEngine::AEMath::Vector;

extern "C" uint8_t g_TextureShader_hasSecondProgram;
extern "C" uint8_t g_TextureShader_activeSlot;

extern "C" void *g_TextureShader_typeInfoSource;
extern "C" void *g_TextureShader_vtable;
extern "C" void *g_TextureShader_typeInfoTarget;

extern "C" void *ShaderBaseStruct_ctor(void *self);
extern "C" void *ShaderBaseStruct_dtor(void *self);
extern "C" uint32_t ShaderBaseStruct_ES2LoadProgram(void *self, const char *vertex, const char *fragment);

extern "C" void String_ctor_char(String *self, const char *text, bool copy);
extern "C" String *String_assign(String *self, const String *other);
extern "C" void String_dtor(String *self);

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

extern "C" float *Vector_to_float(Vector *self);
extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(uint32_t diff);

void operator delete(void *ptr) noexcept;

static inline char *bytes(void *self)
{
    return (char *)self;
}

static inline const char *bytes(const void *self)
{
    return (const char *)self;
}

template <class T>
static inline T &field(void *self, uint32_t offset)
{
    return *(T *)(bytes(self) + offset);
}

template <class T>
static inline const T &field(const void *self, uint32_t offset)
{
    return *(const T *)(bytes(self) + offset);
}

struct TextureShader { void* _opaque; };  // no offset accesses observed
#endif
