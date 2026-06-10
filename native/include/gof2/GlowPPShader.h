#ifndef GOF2_GLOWPPSHADER_H
#define GOF2_GLOWPPSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

struct ShaderBaseStruct;
struct Engine;
struct FBOContainer;
struct Mesh;





} // namespace AbyssEngine

using AbyssEngine::Engine;
using AbyssEngine::FBOContainer;
using AbyssEngine::GlowPPShader;
using AbyssEngine::Mesh;
using AbyssEngine::ShaderBaseStruct;
using AbyssEngine::String;

static_assert(sizeof(String) == 0xc, "String layout");

inline uint8_t &field_u8(void *self, uint32_t off)
{
    return *(uint8_t *)((char *)self + off);
}

inline uint32_t &field_u32(void *self, uint32_t off)
{
    return *(uint32_t *)((char *)self + off);
}

inline int32_t &field_i32(void *self, uint32_t off)
{
    return *(int32_t *)((char *)self + off);
}

inline float &field_float(void *self, uint32_t off)
{
    return *(float *)((char *)self + off);
}

inline void *&field_ptr(void *self, uint32_t off)
{
    return *(void **)((char *)self + off);
}

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

extern "C" __attribute__((visibility("hidden"))) void ShaderBaseStruct_ctor(ShaderBaseStruct *self);
extern "C" __attribute__((visibility("hidden"))) void *ShaderBaseStruct_dtor(ShaderBaseStruct *self);
extern "C" __attribute__((visibility("hidden"))) void String_ctor_char(String *self, const char *text, bool copy);
extern "C" __attribute__((visibility("hidden"))) void String_assign(String *self, const String *other);
extern "C" __attribute__((visibility("hidden"))) void String_dtor(String *self);

extern "C" __attribute__((visibility("hidden"))) void FBOContainer_ctor(FBOContainer *self, Engine *engine, String *name);
extern "C" __attribute__((visibility("hidden"))) void FBOContainer_Create(FBOContainer *self, uint32_t width, uint32_t height, bool depth, bool color);
extern "C" __attribute__((visibility("hidden"))) void FBOContainer_BeginCapture(FBOContainer *self);
extern "C" __attribute__((visibility("hidden"))) void FBOContainer_EndCapture(FBOContainer *self);
extern "C" __attribute__((visibility("hidden"))) void FBOContainer_Activate(FBOContainer *self);

extern "C" uint32_t ShaderBaseStruct_ES2LoadProgram(ShaderBaseStruct *self, const char *vertex, const char *fragment);
extern "C" uint32_t glGetAttribLocation(uint32_t program, const char *name);
extern "C" uint32_t glGetUniformLocation(uint32_t program, const char *name);
extern "C" void glUseProgram(uint32_t program);
extern "C" void glUniform1i(uint32_t location, int32_t value);
extern "C" void glUniform1f(uint32_t location, float value);
extern "C" void glUniformMatrix4fv(uint32_t location, uint32_t count, uint8_t transpose, const void *value);
extern "C" void glEnableVertexAttribArray(uint32_t index);
extern "C" void glDisableVertexAttribArray(uint32_t index);
extern "C" void glVertexAttribPointer(uint32_t index, int32_t size, uint32_t type, uint8_t normalized, uint32_t stride, const void *pointer);
extern "C" void glBindBuffer(uint32_t target, uint32_t buffer);
extern "C" void glDisable(uint32_t cap);
extern "C" void glEnable(uint32_t cap);
extern "C" void glDepthMask(uint8_t flag);
extern "C" void glClearColor(float red, float green, float blue, float alpha);
extern "C" void glClear(uint32_t mask);
extern "C" void glActiveTexture(uint32_t texture);
extern "C" void glBindFramebuffer(uint32_t target, uint32_t framebuffer);
extern "C" void glViewport(int32_t x, int32_t y, uint32_t width, uint32_t height);
extern "C" void glBlendFunc(uint32_t sfactor, uint32_t dfactor);

extern "C" uint32_t Engine_GetDisplayWidth(Engine *self);
extern "C" uint32_t Engine_GetDisplayHeight(Engine *self);
extern "C" void Engine_DrawQuad(Engine *self, int32_t x, int32_t y, uint32_t width);
extern "C" void Engine_SetWorldViewMatrix(Engine *self);

extern "C" uint8_t g_GlowPPShader_internalInitNeeded;
extern "C" uint32_t g_GlowPPShader_shaderMode;
extern "C" uint32_t g_GlowPPShader_vtable;
extern "C" uint32_t g_GlowPPShader_typeInfoSrc;
extern "C" uint32_t g_GlowPPShader_typeInfoDst;

struct GlowPPShader {
    FBOContainer* field_0x3c;           // +0x3c
    FBOContainer* field_0x58;           // +0x58
    FBOContainer* field_0x74;           // +0x74
    FBOContainer* field_0xa0;           // +0xa0
};
#endif
