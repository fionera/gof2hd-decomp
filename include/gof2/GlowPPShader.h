#ifndef GOF2_GLOWPPSHADER_H
#define GOF2_GLOWPPSHADER_H
#include "gof2/common.h"
#include <new>
// Galaxy on Fire 2 -- AbyssEngine::GlowPPShader (Android libgof2hdaa.so, armv7 Thumb).
// GLES2 post-process glow/bloom shader. Holds four FBOContainer render targets (at 0x3c, 0x58,
// 0x74, 0xa0) plus attribute/uniform locations for four sub-programs (copy/blurX/blurY/combine).
// Offset-addressed scalar fields go through the field_* accessor helpers (shared shader-storage idiom).
// The engine entry points are reached through C-linkage thunks declared below.

// ShaderBaseStruct, Engine, Mesh, FBOContainer, GlowPPShader are forward-declared (global)
// in gof2/fwd.h.

// GLES2-call function-pointer types (the shader caches resolved entry points in globals).
typedef uint32_t LoadProgramFn(ShaderBaseStruct *, const char *, const char *);
typedef uint32_t LocationFn(uint32_t, const char *);
typedef void     UseProgramFn(uint32_t);
typedef void     Uniform1iFn(uint32_t, int32_t);

// field_u8 / field_i32 / field_ptr are provided by common.h. Only the helpers
// that common.h does NOT supply are defined here (field_u32, field_float).
inline uint32_t &field_u32(void *self, uint32_t off)
{
    return *(uint32_t *)((char *)self + off);
}

inline float &field_float(void *self, uint32_t off)
{
    return *(float *)((char *)self + off);
}


extern "C" __attribute__((visibility("hidden"))) void ShaderBaseStruct_ctor(ShaderBaseStruct *self);
extern "C" __attribute__((visibility("hidden"))) void *ShaderBaseStruct_dtor(ShaderBaseStruct *self);
// String_ctor_char / String_assign / String_dtor are declared in gof2/String.h
// (included by the .cpp) with their real String*-returning signatures.

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

class GlowPPShader {
public:
    FBOContainer* copyTarget;           // +0x3c  copy target
    FBOContainer* blurXTarget;           // +0x58  blurX target
    FBOContainer* blurYTarget;           // +0x74  blurY target
    FBOContainer* backgroundTarget;           // +0xa0  combine/background target

    // raw field storage for the offset-addressed scalar locations (accessed via field_* helpers)
    char field_storage[0xa8];

    // ---- methods (converted from free functions) ----
    void Init();
    void InternalInit(Engine *engine);
    void RenderEffect(FBOContainer *source, FBOContainer **target, Engine *engine);
    void RenderEffect_simple(FBOContainer *source, Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};
#endif
