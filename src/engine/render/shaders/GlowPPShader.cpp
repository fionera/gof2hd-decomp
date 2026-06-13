#include "gof2/engine/render/shaders/GlowPPShader.h"
#include "gof2/engine/render/ShaderBaseStruct.h"
#include "gof2/externs.h"
#include "gof2/engine/render/Engine.h"
#include "gof2/game/core/String.h"

extern "C" void *operator_new_0(uint32_t size);
extern "C" void *operator_new_1(uint32_t size);
extern "C" void *operator_new_2(uint32_t size);
extern "C" void *operator_new_3(uint32_t size);
extern "C" void String_ctor_char_0(String *self, const char *text, bool copy);
extern "C" void String_ctor_char_1(String *self, const char *text, bool copy);
extern "C" void String_ctor_char_2(String *self, const char *text, bool copy);
extern "C" void String_ctor_char_3(String *self, const char *text, bool copy);
extern "C" void FBOContainer_ctor_0(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_ctor_1(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_ctor_2(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_ctor_3(FBOContainer *self, Engine *engine, String *name);
extern "C" void String_dtor_0(String *self);
extern "C" void String_dtor_1(String *self);
extern "C" void String_dtor_2(String *self);
extern "C" void String_dtor_3(String *self);
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
    glDisableVertexAttribArray(field_u32(this, 0x90));
    return glDisableVertexAttribArray(field_u32(this, 0x98));
}

void GlowPPShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
    glUniformMatrix4fv(field_u32(this, 0x80), 1, 0, (char *)engine + 0x104);
    if (field_u8(this, 0x9) != 0) {
        field_u8(this, 0x9) = 0;
    }

    glEnableVertexAttribArray(field_u32(this, 0x90));
    glEnableVertexAttribArray(field_u32(this, 0x98));
    if (field_u8(mesh, 0x5c) == 0) {
        glVertexAttribPointer(field_u32(this, 0x90), 3, 0x1406, 0, 0, (void *)field_u32(mesh, 4));
        glVertexAttribPointer(field_u32(this, 0x98), 2, 0x1406, 0, 0, (void *)field_u32(mesh, 8));
    } else {
        glBindBuffer(0x8892, field_u32(mesh, 0x60));
        glVertexAttribPointer(field_u32(this, 0x90), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_u32(mesh, 0x68));
        glVertexAttribPointer(field_u32(this, 0x98), 2, 0x1406, 0, 0, 0);
    }
}

void _ZN11AbyssEngine12GlowPPShaderD0Ev(GlowPPShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

static inline FBOContainer *fbo_field(GlowPPShader *self, uint32_t off)
{
    return *(FBOContainer **)((char *)self + off);
}

static inline void draw_fullscreen(GlowPPShader *self, Engine *engine, uint32_t posOff,
                                   uint32_t texOff, uint32_t matrixOff)
{
    glEnableVertexAttribArray(field_u32(self, posOff));
    glEnableVertexAttribArray(field_u32(self, texOff));
    glUniformMatrix4fv(field_u32(self, matrixOff), 1, 0, (char *)engine + 0x104);
    glVertexAttribPointer(field_u32(self, posOff), 3, 0x1406, 0, 0,
                          (void *)field_u32((void *)field_u32(engine, 0x380), 4));
    glVertexAttribPointer(field_u32(self, texOff), 2, 0x1406, 0, 0,
                          (void *)field_u32((void *)field_u32(engine, 0x380), 8));
    glClear(0x4000);
    uint32_t width = ((Engine *)(engine))->GetDisplayWidth();
    uint32_t height = ((Engine *)(engine))->GetDisplayHeight();
    ((Engine *)(engine))->DrawQuad(0, 0, width, height);
    glDisableVertexAttribArray(field_u32(self, posOff));
    glDisableVertexAttribArray(field_u32(self, texOff));
}

void GlowPPShader::RenderEffect(FBOContainer *source, FBOContainer **target, Engine *engine) {

    if (*g_GlowPPShader_internalInitNeededPtr != 0) {
        *g_GlowPPShader_internalInitNeededPtr = 0;
        ((GlowPPShader *)(this))->InternalInit(engine);
        ((FBOContainer *)(fbo_field(this, 0xa0)))->BeginCapture();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(0x4000);
        ((FBOContainer *)(fbo_field(this, 0xa0)))->EndCapture();
    }

    field_u32(engine, 0x3b4) = 0;
    field_u32(engine, 0x3b8) = 0;
    field_u32(engine, 0x3bc) = 0;
    field_u32(engine, 0x3c0) = 0;
    field_u32(engine, 0x3a4) = 0;
    field_u32(engine, 0x3a8) = 0;
    field_u32(engine, 0x3ac) = 0;
    field_u32(engine, 0x3b0) = 0;
    field_u32(engine, 0x394) = 0;
    field_u32(engine, 0x398) = 0;
    field_u32(engine, 0x39c) = 0;
    field_u32(engine, 0x3a0) = 0;
    field_u32(engine, 0x384) = 0;
    field_u32(engine, 0x388) = 0;
    field_u32(engine, 0x38c) = 0;
    field_u32(engine, 0x390) = 0;

    field_float(engine, 0x384) = 2.0f / (float)(int32_t)((Engine *)(engine))->GetDisplayWidth();
    field_float(engine, 0x398) = -(2.0f / (float)(int32_t)((Engine *)(engine))->GetDisplayHeight());
    field_u32(engine, 0x3ac) = 0xbf800000;
    field_u32(engine, 0x3b4) = 0xbf800000;
    field_u32(engine, 0x3b8) = 0x3f800000;
    field_u32(engine, 0x3c0) = 0x3f800000;

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

    glUseProgram(field_u32(this, 0x20));
    glActiveTexture(0x84c0);
    ((FBOContainer *)(source))->Activate();
    ((FBOContainer *)(fbo_field(this, 0x3c)))->BeginCapture();
    draw_fullscreen(this, engine, 0x24, 0x2c, 0x28);

    for (int32_t i = 3; i != 0; --i) {
        glUseProgram(field_u32(this, 0x40));
        glActiveTexture(0x84c0);
        ((FBOContainer *)(fbo_field(this, 0x3c)))->Activate();
        ((FBOContainer *)(fbo_field(this, 0x58)))->BeginCapture();
        glEnableVertexAttribArray(field_u32(this, 0x44));
        glEnableVertexAttribArray(field_u32(this, 0x4c));
        glUniformMatrix4fv(field_u32(this, 0x48), 1, 0, (char *)engine + 0x104);
        glVertexAttribPointer(field_u32(this, 0x44), 3, 0x1406, 0, 0,
                              (void *)field_u32((void *)field_u32(engine, 0x380), 4));
        glVertexAttribPointer(field_u32(this, 0x4c), 2, 0x1406, 0, 0,
                              (void *)field_u32((void *)field_u32(engine, 0x380), 8));
        glUniform1f(field_u32(this, 0x54),
                    1.0f / (float)field_i32((void *)field_u32(this, 0x58), 0x0c));
        glClear(0x4000);
        uint32_t width = ((Engine *)(engine))->GetDisplayWidth();
        uint32_t height = ((Engine *)(engine))->GetDisplayHeight();
        ((Engine *)(engine))->DrawQuad(0, 0, width, height);
        glDisableVertexAttribArray(field_u32(this, 0x44));
        glDisableVertexAttribArray(field_u32(this, 0x4c));

        glUseProgram(field_u32(this, 0x5c));
        glActiveTexture(0x84c0);
        ((FBOContainer *)(fbo_field(this, 0x58)))->Activate();
        ((FBOContainer *)(fbo_field(this, 0x74)))->BeginCapture();
        glEnableVertexAttribArray(field_u32(this, 0x60));
        glEnableVertexAttribArray(field_u32(this, 0x68));
        glUniformMatrix4fv(field_u32(this, 0x64), 1, 0, (char *)engine + 0x104);
        glVertexAttribPointer(field_u32(this, 0x60), 3, 0x1406, 0, 0,
                              (void *)field_u32((void *)field_u32(engine, 0x380), 4));
        glVertexAttribPointer(field_u32(this, 0x68), 2, 0x1406, 0, 0,
                              (void *)field_u32((void *)field_u32(engine, 0x380), 8));
        glUniform1f(field_u32(this, 0x70),
                    1.0f / (float)field_i32((void *)field_u32(this, 0x74), 0x10));
        glClear(0x4000);
        width = ((Engine *)(engine))->GetDisplayWidth();
        height = ((Engine *)(engine))->GetDisplayHeight();
        ((Engine *)(engine))->DrawQuad(0, 0, width, height);
        glDisableVertexAttribArray(field_u32(this, 0x60));
        glDisableVertexAttribArray(field_u32(this, 0x68));
    }

    FBOContainer *firstTexture = fbo_field(this, 0x74);
    FBOContainer *secondTexture = fbo_field(this, 0xa0);
    uint32_t mode = *g_GlowPPShader_shaderModePtr;
    if (mode == 0) {
        firstTexture = fbo_field(this, 0x3c);
    } else if (mode == 1) {
        firstTexture = source;
    }

    glUseProgram(field_u32(this, 0x78));
    glActiveTexture(0x84c0);
    ((FBOContainer *)(firstTexture))->Activate();
    glActiveTexture(0x84c1);
    ((FBOContainer *)(secondTexture))->Activate();

    if (*target == 0) {
        glBindFramebuffer(0x8d40, field_u32(engine, 0x40c));
        uint32_t width;
        uint32_t height;
        if (field_i32((void *)field_u32((void *)field_u32(engine, 0x30), 0), 0x30) == 2) {
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

    draw_fullscreen(this, engine, 0x7c, 0x84, 0x80);
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
    String_ctor_char_0(&name0, "GlowPPShader0", false);
    FBOContainer_ctor_0(fbo, engine, &name0);
    this->copyTarget = fbo;
    String_dtor_0(&name0);
    FBOContainer_Create_0(this->copyTarget, 0x200, 0x200, true, false);

    fbo = (FBOContainer *)operator_new_1(0x38);
    String_ctor_char_1(&name1, "GlowPPShader1", false);
    FBOContainer_ctor_1(fbo, engine, &name1);
    this->blurXTarget = fbo;
    String_dtor_1(&name1);
    FBOContainer_Create_1(this->blurXTarget, 0x200, 0x200, true, false);

    fbo = (FBOContainer *)operator_new_2(0x38);
    String_ctor_char_2(&name2, "GlowPPShader2", false);
    FBOContainer_ctor_2(fbo, engine, &name2);
    this->blurYTarget = fbo;
    String_dtor_2(&name2);
    FBOContainer_Create_2(this->blurYTarget, 0x200, 0x200, true, false);

    fbo = (FBOContainer *)operator_new_3(0x38);
    String_ctor_char_3(&name3, "GlowPPShader3", false);
    FBOContainer_ctor_3(fbo, engine, &name3);
    this->backgroundTarget = fbo;
    String_dtor_3(&name3);
    FBOContainer_Create_3(this->backgroundTarget, 0x200, 0x200, true, false);

    return;
}

typedef uint32_t LoadProgramFn(ShaderBaseStruct *, const char *, const char *);
typedef uint32_t LocationFn(uint32_t, const char *);
typedef void UseProgramFn(uint32_t);
typedef void Uniform1iFn(uint32_t, int32_t);

void GlowPPShader::Init() {
    const char *vertex = "GlowPPShader.vert";
    LoadProgramFn *loadProgram = g_GlowPPShader_LoadProgram;
    field_u32(this, 0x20) = loadProgram((ShaderBaseStruct *)this, vertex, "GlowPPShader.copy.frag");
    field_u32(this, 0x40) = loadProgram((ShaderBaseStruct *)this, vertex, "GlowPPShader.blurX.frag");
    field_u32(this, 0x5c) = loadProgram((ShaderBaseStruct *)this, vertex, "GlowPPShader.blurY.frag");
    field_u32(this, 0x78) = loadProgram((ShaderBaseStruct *)this, vertex, "GlowPPShader.combine.frag");

    LocationFn *getAttribLocation = g_GlowPPShader_GetAttribLocation;
    const char *position = "position";
    field_u32(this, 0x24) = getAttribLocation(field_u32(this, 0x20), position);
    const char *texcoord = "texcoord";
    field_u32(this, 0x2c) = getAttribLocation(field_u32(this, 0x20), texcoord);

    LocationFn *getUniformLocation = g_GlowPPShader_GetUniformLocation;
    const char *worldView = "worldView";
    field_u32(this, 0x28) = getUniformLocation(field_u32(this, 0x20), worldView);
    const char *texture = "texture";
    field_u32(this, 0x30) = getUniformLocation(field_u32(this, 0x20), texture);

    UseProgramFn *useProgram = g_GlowPPShader_UseProgram;
    useProgram(field_u32(this, 0x20));
    Uniform1iFn *uniform1i = g_GlowPPShader_Uniform1i;
    uniform1i(field_u32(this, 0x30), 0);

    field_u32(this, 0x44) = getAttribLocation(field_u32(this, 0x40), position);
    field_u32(this, 0x4c) = getAttribLocation(field_u32(this, 0x40), texcoord);
    field_u32(this, 0x48) = getUniformLocation(field_u32(this, 0x40), worldView);
    field_u32(this, 0x50) = getUniformLocation(field_u32(this, 0x40), texture);
    const char *sampleSize = "sampleSize";
    field_u32(this, 0x54) = getUniformLocation(field_u32(this, 0x40), sampleSize);
    useProgram(field_u32(this, 0x40));
    uniform1i(field_u32(this, 0x50), 0);

    field_u32(this, 0x60) = getAttribLocation(field_u32(this, 0x5c), position);
    field_u32(this, 0x68) = getAttribLocation(field_u32(this, 0x5c), texcoord);
    field_u32(this, 0x64) = getUniformLocation(field_u32(this, 0x5c), worldView);
    field_u32(this, 0x6c) = getUniformLocation(field_u32(this, 0x5c), texture);
    field_u32(this, 0x70) = getUniformLocation(field_u32(this, 0x5c), sampleSize);
    useProgram(field_u32(this, 0x5c));
    uniform1i(field_u32(this, 0x6c), 0);

    field_u32(this, 0x7c) = getAttribLocation(field_u32(this, 0x78), position);
    field_u32(this, 0x84) = getAttribLocation(field_u32(this, 0x78), texcoord);
    field_u32(this, 0x80) = getUniformLocation(field_u32(this, 0x78), worldView);
    field_u32(this, 0x88) = getUniformLocation(field_u32(this, 0x78), texture);
    const char *texture2 = "texture2";
    field_u32(this, 0x8c) = getUniformLocation(field_u32(this, 0x78), texture2);
    useProgram(field_u32(this, 0x78));
    uniform1i(field_u32(this, 0x88), 0);
    return uniform1i(field_u32(this, 0x8c), 1);
}

GlowPPShader *_ZN11AbyssEngine12GlowPPShaderC1Ev(GlowPPShader *self)
{
    void *name[3];
    name[0] = self;
    new ((AbyssEngine::ShaderBaseStruct *)self) AbyssEngine::ShaderBaseStruct();
    void **source = (void **)GlowPPShader_typeinfo_source;
    void **dest = (void **)GlowPPShader_typeinfo_dest;
    *(void **)self = (char *)GlowPPShader_vtable + 8;
    *dest = *source;
    ((String *)((String *)name))->ctor_char("GlowPPShader", false);
    ((String *)((String *)((char *)self + 0xc)))->assign((String *)name);
    ((String *)((String *)name))->dtor();
    return self;
}
