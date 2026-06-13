#include "gof2/engine/render/shaders/GlowPPShader.h"
#include "gof2/externs.h"
#include "gof2/engine/render/Engine.h"
#include "gof2/game/core/String.h"

// cross-class field accessors (Engine/Mesh/FBOContainer are not in this batch; opaque here)
template <class T> static inline T &ae_field(void *base, unsigned int off) {
    return *(T *)((char *)base + off);
}

extern "C" void *operator_new_0(uint32_t size);
extern "C" void *operator_new_1(uint32_t size);
extern "C" void *operator_new_2(uint32_t size);
extern "C" void *operator_new_3(uint32_t size);
extern "C" void FBOContainer_ctor_0(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_ctor_1(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_ctor_2(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_ctor_3(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_Create_0(FBOContainer *self, uint32_t width, uint32_t height, bool depth, bool color);
extern "C" void FBOContainer_Create_1(FBOContainer *self, uint32_t width, uint32_t height, bool depth, bool color);
extern "C" void FBOContainer_Create_2(FBOContainer *self, uint32_t width, uint32_t height, bool depth, bool color);
extern "C" void FBOContainer_Create_3(FBOContainer *self, uint32_t width, uint32_t height, bool depth, bool color);
extern "C" __attribute__((visibility("hidden"))) LoadProgramFn *g_GlowPPShader_LoadProgram;
extern "C" __attribute__((visibility("hidden"))) LocationFn *g_GlowPPShader_GetAttribLocation;
extern "C" __attribute__((visibility("hidden"))) LocationFn *g_GlowPPShader_GetUniformLocation;
extern "C" __attribute__((visibility("hidden"))) UseProgramFn *g_GlowPPShader_UseProgram;
extern "C" __attribute__((visibility("hidden"))) Uniform1iFn *g_GlowPPShader_Uniform1i;
extern "C" void *GlowPPShader_vtable;
extern "C" void *GlowPPShader_typeinfo_source;
extern "C" void *GlowPPShader_typeinfo_dest;

void GlowPPShader::SetInActive() {
    glDisableVertexAttribArray(this->meshAttribPosition);
    return glDisableVertexAttribArray(this->meshAttribTexCoord);
}

void GlowPPShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
    glUniformMatrix4fv(this->combineUniformWorldView, 1, 0, (char *)engine + 0x104);
    if (this->dirty != 0) {
        this->dirty = 0;
    }

    glEnableVertexAttribArray(this->meshAttribPosition);
    glEnableVertexAttribArray(this->meshAttribTexCoord);
    if (ae_field<uint8_t>(mesh, 0x5c) == 0) {
        glVertexAttribPointer(this->meshAttribPosition, 3, 0x1406, 0, 0, (void *)ae_field<uint32_t>(mesh, 4));
        glVertexAttribPointer(this->meshAttribTexCoord, 2, 0x1406, 0, 0, (void *)ae_field<uint32_t>(mesh, 8));
    } else {
        glBindBuffer(0x8892, ae_field<uint32_t>(mesh, 0x60));
        glVertexAttribPointer(this->meshAttribPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, ae_field<uint32_t>(mesh, 0x68));
        glVertexAttribPointer(this->meshAttribTexCoord, 2, 0x1406, 0, 0, 0);
    }
}

void _ZN11AbyssEngine12GlowPPShaderD0Ev(GlowPPShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

static inline void draw_fullscreen(GlowPPShader *self, Engine *engine, int posLoc,
                                   int texLoc, int matrixLoc)
{
    glEnableVertexAttribArray(posLoc);
    glEnableVertexAttribArray(texLoc);
    glUniformMatrix4fv(matrixLoc, 1, 0, (char *)engine + 0x104);
    glVertexAttribPointer(posLoc, 3, 0x1406, 0, 0,
                          (void *)ae_field<uint32_t>((void *)ae_field<uint32_t>(engine, 0x380), 4));
    glVertexAttribPointer(texLoc, 2, 0x1406, 0, 0,
                          (void *)ae_field<uint32_t>((void *)ae_field<uint32_t>(engine, 0x380), 8));
    glClear(0x4000);
    uint32_t width = ((Engine *)(engine))->GetDisplayWidth();
    uint32_t height = ((Engine *)(engine))->GetDisplayHeight();
    ((Engine *)(engine))->DrawQuad(0, 0, width, height);
    glDisableVertexAttribArray(posLoc);
    glDisableVertexAttribArray(texLoc);
}

void GlowPPShader::RenderEffect(FBOContainer *source, FBOContainer **target, Engine *engine) {

    if (*g_GlowPPShader_internalInitNeededPtr != 0) {
        *g_GlowPPShader_internalInitNeededPtr = 0;
        ((GlowPPShader *)(this))->InternalInit(engine);
        ((FBOContainer *)(this->backgroundTarget))->BeginCapture();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(0x4000);
        ((FBOContainer *)(this->backgroundTarget))->EndCapture();
    }

    ae_field<uint32_t>(engine, 0x3b4) = 0;
    ae_field<uint32_t>(engine, 0x3b8) = 0;
    ae_field<uint32_t>(engine, 0x3bc) = 0;
    ae_field<uint32_t>(engine, 0x3c0) = 0;
    ae_field<uint32_t>(engine, 0x3a4) = 0;
    ae_field<uint32_t>(engine, 0x3a8) = 0;
    ae_field<uint32_t>(engine, 0x3ac) = 0;
    ae_field<uint32_t>(engine, 0x3b0) = 0;
    ae_field<uint32_t>(engine, 0x394) = 0;
    ae_field<uint32_t>(engine, 0x398) = 0;
    ae_field<uint32_t>(engine, 0x39c) = 0;
    ae_field<uint32_t>(engine, 0x3a0) = 0;
    ae_field<uint32_t>(engine, 0x384) = 0;
    ae_field<uint32_t>(engine, 0x388) = 0;
    ae_field<uint32_t>(engine, 0x38c) = 0;
    ae_field<uint32_t>(engine, 0x390) = 0;

    ae_field<float>(engine, 0x384) = 2.0f / (float)(int32_t)((Engine *)(engine))->GetDisplayWidth();
    ae_field<float>(engine, 0x398) = -(2.0f / (float)(int32_t)((Engine *)(engine))->GetDisplayHeight());
    ae_field<uint32_t>(engine, 0x3ac) = 0xbf800000;
    ae_field<uint32_t>(engine, 0x3b4) = 0xbf800000;
    ae_field<uint32_t>(engine, 0x3b8) = 0x3f800000;
    ae_field<uint32_t>(engine, 0x3c0) = 0x3f800000;

    float matrix[16];
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
    ((Engine *)(engine))->SetWorldViewMatrix((const uint32_t *)matrix);
    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);

    glUseProgram(this->copyProgram);
    glActiveTexture(0x84c0);
    ((FBOContainer *)(source))->Activate();
    ((FBOContainer *)(this->copyTarget))->BeginCapture();
    draw_fullscreen(this, engine, this->copyAttribPosition, this->copyAttribTexCoord, this->copyUniformWorldView);

    for (int32_t i = 3; i != 0; --i) {
        glUseProgram(this->blurXProgram);
        glActiveTexture(0x84c0);
        ((FBOContainer *)(this->copyTarget))->Activate();
        ((FBOContainer *)(this->blurXTarget))->BeginCapture();
        glEnableVertexAttribArray(this->blurXAttribPosition);
        glEnableVertexAttribArray(this->blurXAttribTexCoord);
        glUniformMatrix4fv(this->blurXUniformWorldView, 1, 0, (char *)engine + 0x104);
        glVertexAttribPointer(this->blurXAttribPosition, 3, 0x1406, 0, 0,
                              (void *)ae_field<uint32_t>((void *)ae_field<uint32_t>(engine, 0x380), 4));
        glVertexAttribPointer(this->blurXAttribTexCoord, 2, 0x1406, 0, 0,
                              (void *)ae_field<uint32_t>((void *)ae_field<uint32_t>(engine, 0x380), 8));
        glUniform1f(this->blurXUniformSampleSize,
                    1.0f / (float)ae_field<int32_t>((void *)this->blurXTarget, 0x0c));
        glClear(0x4000);
        uint32_t width = ((Engine *)(engine))->GetDisplayWidth();
        uint32_t height = ((Engine *)(engine))->GetDisplayHeight();
        ((Engine *)(engine))->DrawQuad(0, 0, width, height);
        glDisableVertexAttribArray(this->blurXAttribPosition);
        glDisableVertexAttribArray(this->blurXAttribTexCoord);

        glUseProgram(this->blurYProgram);
        glActiveTexture(0x84c0);
        ((FBOContainer *)(this->blurXTarget))->Activate();
        ((FBOContainer *)(this->blurYTarget))->BeginCapture();
        glEnableVertexAttribArray(this->blurYAttribPosition);
        glEnableVertexAttribArray(this->blurYAttribTexCoord);
        glUniformMatrix4fv(this->blurYUniformWorldView, 1, 0, (char *)engine + 0x104);
        glVertexAttribPointer(this->blurYAttribPosition, 3, 0x1406, 0, 0,
                              (void *)ae_field<uint32_t>((void *)ae_field<uint32_t>(engine, 0x380), 4));
        glVertexAttribPointer(this->blurYAttribTexCoord, 2, 0x1406, 0, 0,
                              (void *)ae_field<uint32_t>((void *)ae_field<uint32_t>(engine, 0x380), 8));
        glUniform1f(this->blurYUniformSampleSize,
                    1.0f / (float)ae_field<int32_t>((void *)this->blurYTarget, 0x10));
        glClear(0x4000);
        width = ((Engine *)(engine))->GetDisplayWidth();
        height = ((Engine *)(engine))->GetDisplayHeight();
        ((Engine *)(engine))->DrawQuad(0, 0, width, height);
        glDisableVertexAttribArray(this->blurYAttribPosition);
        glDisableVertexAttribArray(this->blurYAttribTexCoord);
    }

    FBOContainer *firstTexture = this->blurYTarget;
    FBOContainer *secondTexture = this->backgroundTarget;
    uint32_t mode = *g_GlowPPShader_shaderModePtr;
    if (mode == 0) {
        firstTexture = this->copyTarget;
    } else if (mode == 1) {
        firstTexture = source;
    }

    glUseProgram(this->combineProgram);
    glActiveTexture(0x84c0);
    ((FBOContainer *)(firstTexture))->Activate();
    glActiveTexture(0x84c1);
    ((FBOContainer *)(secondTexture))->Activate();

    if (*target == 0) {
        glBindFramebuffer(0x8d40, ae_field<uint32_t>(engine, 0x40c));
        uint32_t width;
        uint32_t height;
        if (ae_field<int32_t>((void *)ae_field<uint32_t>((void *)ae_field<uint32_t>(engine, 0x30), 0), 0x30) == 2) {
            width = ((Engine *)(engine))->GetDisplayWidth();
            height = ((Engine *)(engine))->GetDisplayHeight();
        } else {
            width = ((Engine *)(engine))->GetDisplayHeight();
            height = ((Engine *)(engine))->GetDisplayWidth();
        }
        glViewport(0, 0, width, height);
    } else {
        ((FBOContainer *)(*target))->BeginCapture();
    }

    draw_fullscreen(this, engine, this->combineAttribPosition, this->combineAttribTexCoord, this->combineUniformWorldView);
    if (*target != 0) {
        ((FBOContainer *)(*target))->EndCapture();
    }

    glEnable(0xbe2);
    glBlendFunc(0x302, 0x303);
    glActiveTexture(0x84c0);

    return;
}

typedef void RenderEffectFn(GlowPPShader *, FBOContainer *, FBOContainer **, Engine *);

static inline uint32_t stack_guard_diff(uint32_t saved, uint32_t current)
{
    return current - saved;
}

void GlowPPShader::RenderEffect_simple(FBOContainer *source, Engine *engine) {
    FBOContainer *target = 0;
    void **vtable = *(void ***)this;
    ((RenderEffectFn *)vtable[0x1c / 4])(this, source, &target, engine);
    return;
}

void GlowPPShader::InternalInit(Engine *engine) {
    String name0;
    String name1;
    String name2;
    String name3;

    FBOContainer *fbo = (FBOContainer *)operator_new_0(0x38);
    name0.ctor_char("GlowPPShader0", false);
    FBOContainer_ctor_0(fbo, engine, &name0);
    this->copyTarget = fbo;
    name0.dtor();
    FBOContainer_Create_0(this->copyTarget, 0x200, 0x200, true, false);

    fbo = (FBOContainer *)operator_new_1(0x38);
    name1.ctor_char("GlowPPShader1", false);
    FBOContainer_ctor_1(fbo, engine, &name1);
    this->blurXTarget = fbo;
    name1.dtor();
    FBOContainer_Create_1(this->blurXTarget, 0x200, 0x200, true, false);

    fbo = (FBOContainer *)operator_new_2(0x38);
    name2.ctor_char("GlowPPShader2", false);
    FBOContainer_ctor_2(fbo, engine, &name2);
    this->blurYTarget = fbo;
    name2.dtor();
    FBOContainer_Create_2(this->blurYTarget, 0x200, 0x200, true, false);

    fbo = (FBOContainer *)operator_new_3(0x38);
    name3.ctor_char("GlowPPShader3", false);
    FBOContainer_ctor_3(fbo, engine, &name3);
    this->backgroundTarget = fbo;
    name3.dtor();
    FBOContainer_Create_3(this->backgroundTarget, 0x200, 0x200, true, false);

    return;
}

void GlowPPShader::Init() {
    const char *vertex = "GlowPPShader.vert";
    LoadProgramFn *loadProgram = g_GlowPPShader_LoadProgram;
    this->copyProgram = loadProgram((AbyssEngine::ShaderBaseStruct *)this, vertex, "GlowPPShader.copy.frag");
    this->blurXProgram = loadProgram((AbyssEngine::ShaderBaseStruct *)this, vertex, "GlowPPShader.blurX.frag");
    this->blurYProgram = loadProgram((AbyssEngine::ShaderBaseStruct *)this, vertex, "GlowPPShader.blurY.frag");
    this->combineProgram = loadProgram((AbyssEngine::ShaderBaseStruct *)this, vertex, "GlowPPShader.combine.frag");

    LocationFn *getAttribLocation = g_GlowPPShader_GetAttribLocation;
    const char *position = "position";
    this->copyAttribPosition = getAttribLocation(this->copyProgram, position);
    const char *texcoord = "texcoord";
    this->copyAttribTexCoord = getAttribLocation(this->copyProgram, texcoord);

    LocationFn *getUniformLocation = g_GlowPPShader_GetUniformLocation;
    const char *worldView = "worldView";
    this->copyUniformWorldView = getUniformLocation(this->copyProgram, worldView);
    const char *texture = "texture";
    this->copyUniformTexture = getUniformLocation(this->copyProgram, texture);

    UseProgramFn *useProgram = g_GlowPPShader_UseProgram;
    useProgram(this->copyProgram);
    Uniform1iFn *uniform1i = g_GlowPPShader_Uniform1i;
    uniform1i(this->copyUniformTexture, 0);

    this->blurXAttribPosition = getAttribLocation(this->blurXProgram, position);
    this->blurXAttribTexCoord = getAttribLocation(this->blurXProgram, texcoord);
    this->blurXUniformWorldView = getUniformLocation(this->blurXProgram, worldView);
    this->blurXUniformTexture = getUniformLocation(this->blurXProgram, texture);
    const char *sampleSize = "sampleSize";
    this->blurXUniformSampleSize = getUniformLocation(this->blurXProgram, sampleSize);
    useProgram(this->blurXProgram);
    uniform1i(this->blurXUniformTexture, 0);

    this->blurYAttribPosition = getAttribLocation(this->blurYProgram, position);
    this->blurYAttribTexCoord = getAttribLocation(this->blurYProgram, texcoord);
    this->blurYUniformWorldView = getUniformLocation(this->blurYProgram, worldView);
    this->blurYUniformTexture = getUniformLocation(this->blurYProgram, texture);
    this->blurYUniformSampleSize = getUniformLocation(this->blurYProgram, sampleSize);
    useProgram(this->blurYProgram);
    uniform1i(this->blurYUniformTexture, 0);

    this->combineAttribPosition = getAttribLocation(this->combineProgram, position);
    this->combineAttribTexCoord = getAttribLocation(this->combineProgram, texcoord);
    this->combineUniformWorldView = getUniformLocation(this->combineProgram, worldView);
    this->combineUniformTexture = getUniformLocation(this->combineProgram, texture);
    const char *texture2 = "texture2";
    this->combineUniformTexture2 = getUniformLocation(this->combineProgram, texture2);
    useProgram(this->combineProgram);
    uniform1i(this->combineUniformTexture, 0);
    return uniform1i(this->combineUniformTexture2, 1);
}

GlowPPShader *_ZN11AbyssEngine12GlowPPShaderC1Ev(GlowPPShader *self)
{
    String name;
    new ((AbyssEngine::ShaderBaseStruct *)self) AbyssEngine::ShaderBaseStruct();
    void **source = (void **)GlowPPShader_typeinfo_source;
    void **dest = (void **)GlowPPShader_typeinfo_dest;
    self->field_0x0 = (char *)GlowPPShader_vtable + 8;
    *dest = *source;
    name.ctor_char("GlowPPShader", false);
    self->name.assign(&name);
    name.dtor();
    return self;
}
