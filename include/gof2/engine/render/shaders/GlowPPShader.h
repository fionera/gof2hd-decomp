#ifndef GOF2_GLOWPPSHADER_H
#define GOF2_GLOWPPSHADER_H
#include "gof2/common.h"
#include <new>
// Galaxy on Fire 2 -- AbyssEngine::GlowPPShader (Android libgof2hdaa.so, armv7 Thumb).
// GLES2 post-process glow/bloom shader. Derives from ShaderBaseStruct and owns four
// FBOContainer render targets plus attribute/uniform locations for four sub-programs
// (copy/blurX/blurY/combine). The engine entry points are reached through C-linkage
// thunks declared below.

namespace AbyssEngine {

// AbyssEngine::ShaderBaseStruct base layout used by GlowPPShader (truncated local copy;
// the canonical class lives in ShaderBaseStruct.h). Only the leading trivial fields are
// modelled here so the placement-new in the ctor constructs just those; the non-trivial
// String name and the per-shader location members below are real derived members.
struct ShaderBaseStruct {
    void *field_0x0;                    // +0x0 vtable
    int field_0x4;                      // +0x4 GL program handle (copy)
    volatile uint16_t field_0x8;        // +0x8 flags

    static int shaderIndexIntern;

    ShaderBaseStruct();
    ~ShaderBaseStruct();
    uint32_t ES2LoadProgram(const char *vertexSource, const char *fragmentSource);
};

} // namespace AbyssEngine

// GLES2-call function-pointer types (the shader caches resolved entry points in globals).
typedef uint32_t LoadProgramFn(AbyssEngine::ShaderBaseStruct *, const char *, const char *);
typedef uint32_t LocationFn(uint32_t, const char *);
typedef void     UseProgramFn(uint32_t);
typedef void     Uniform1iFn(uint32_t, int32_t);

// Minimal view of FBOContainer (full definition lives in gof2/FBOContainer.h, owned by
// another batch). Here we only need the three render-to-texture capture methods invoked
// from GlowPPShader's post-process passes.
class FBOContainer {
public:
    void BeginCapture();
    void EndCapture();
    void Activate();
};

extern "C" __attribute__((visibility("hidden"))) void FBOContainer_ctor(FBOContainer *self, Engine *engine, String *name);
extern "C" __attribute__((visibility("hidden"))) void FBOContainer_Create(FBOContainer *self, uint32_t width, uint32_t height, bool depth, bool color);
extern "C" __attribute__((visibility("hidden"))) void FBOContainer_BeginCapture(FBOContainer *self);
extern "C" __attribute__((visibility("hidden"))) void FBOContainer_EndCapture(FBOContainer *self);
extern "C" __attribute__((visibility("hidden"))) void FBOContainer_Activate(FBOContainer *self);

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

// AbyssEngine::GlowPPShader.
// Field 0x0/0x4 live in the ShaderBaseStruct base (vtable + copy program handle).
class GlowPPShader : public AbyssEngine::ShaderBaseStruct {
public:
    uint8_t dirty;                  // +0x9  per-frame uniform-upload gate
    String name;                    // +0xc  shader name

    unsigned int copyProgram;       // +0x20 GL program handle (copy)
    int copyAttribPosition;         // +0x24 position
    int copyUniformWorldView;       // +0x28 worldView
    int copyAttribTexCoord;         // +0x2c texcoord
    int copyUniformTexture;         // +0x30 texture

    FBOContainer *copyTarget;       // +0x3c copy render target

    unsigned int blurXProgram;      // +0x40 GL program handle (blurX)
    int blurXAttribPosition;        // +0x44 position
    int blurXUniformWorldView;      // +0x48 worldView
    int blurXAttribTexCoord;        // +0x4c texcoord
    int blurXUniformTexture;        // +0x50 texture
    int blurXUniformSampleSize;     // +0x54 sampleSize

    FBOContainer *blurXTarget;      // +0x58 blurX render target

    unsigned int blurYProgram;      // +0x5c GL program handle (blurY)
    int blurYAttribPosition;        // +0x60 position
    int blurYUniformWorldView;      // +0x64 worldView
    int blurYAttribTexCoord;        // +0x68 texcoord
    int blurYUniformTexture;        // +0x6c texture
    int blurYUniformSampleSize;     // +0x70 sampleSize

    FBOContainer *blurYTarget;      // +0x74 blurY render target

    unsigned int combineProgram;    // +0x78 GL program handle (combine)
    int combineAttribPosition;      // +0x7c position
    int combineUniformWorldView;    // +0x80 worldView
    int combineAttribTexCoord;      // +0x84 texcoord
    int combineUniformTexture;      // +0x88 texture
    int combineUniformTexture2;     // +0x8c texture2

    int meshAttribPosition;         // +0x90 attribute used in UpdateMeshData/SetInActive
    int pad_0x94;                   // +0x94
    int meshAttribTexCoord;         // +0x98 attribute used in UpdateMeshData/SetInActive
    int pad_0x9c;                   // +0x9c

    FBOContainer *backgroundTarget; // +0xa0 combine/background render target

    GlowPPShader();
    void Init();
    void InternalInit(Engine *engine);
    void RenderEffect(FBOContainer *source, FBOContainer **target, Engine *engine);
    void RenderEffect_simple(FBOContainer *source, Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};
#endif
