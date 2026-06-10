#include "BloomShader.h"


extern "C" int glGetAttribLocation(unsigned int program, const char *name);
extern "C" int glGetUniformLocation(unsigned int program, const char *name);
extern "C" void glUseProgram(unsigned int program);
extern "C" void glUniform1i(int location, int value);
extern "C" void *_ZN11AbyssEngine11BloomShaderD1Ev(BloomShader *self);
extern "C" void operator_delete(void *self);
extern "C" void glEnableVertexAttribArray(unsigned int index);
extern "C" void glBindBuffer(unsigned int target, unsigned int buffer);
extern "C" void glDisable(unsigned int cap);
extern "C" void glEnable(unsigned int cap);
extern "C" void glDepthMask(unsigned char flag);
extern "C" void glActiveTexture(unsigned int texture);
extern "C" void glBindFramebuffer(unsigned int target, unsigned int framebuffer);
extern "C" void glViewport(int x, int y, unsigned int width, unsigned int height);
extern "C" void glDisableVertexAttribArray(unsigned int index);
extern "C" void glUniform1f(int location, float value);
extern "C" void glClearColor(float red, float green, float blue, float alpha);
extern "C" void glClear(unsigned int mask);
extern "C" void glBlendFunc(unsigned int sfactor, unsigned int dfactor);
extern "C" int Engine_GetDisplayWidth(Engine *self);
extern "C" int Engine_GetDisplayHeight(Engine *self);
extern "C" void Engine_SetWorldViewMatrix(Engine *self);
extern "C" void FBOContainer_Activate(FBOContainer *self);
extern "C" void FBOContainer_BeginCapture(FBOContainer *self);
extern "C" void FBOContainer_EndCapture(FBOContainer *self);
extern "C" unsigned char g_BloomShader_internalInitNeeded;
extern "C" unsigned int g_BloomShader_shaderMode;
extern "C" void glDisableVertexAttribArray_thunk(unsigned int index);
extern "C" void ShaderBaseStruct_ctor(ShaderBaseStruct *self);
extern "C" char BloomShader_vtable;
extern "C" void *BloomShader_typeinfo_source;
extern "C" void *BloomShader_typeinfo_dest;
extern "C" void FBOContainer_ctor(FBOContainer *self, Engine *engine, String *name);
extern "C" void *ShaderBaseStruct_dtor(ShaderBaseStruct *self);

// ---- Init_895ec.cpp ----
extern "C" unsigned int ShaderBaseStruct_ES2LoadProgram(ShaderBaseStruct *self,
                                                        const char *vertex,
                                                        const char *fragment);

namespace AbyssEngine {

void BloomShader::Init(Engine *)
{
    unsigned int (*loadProgram)(ShaderBaseStruct *, const char *, const char *) =
        ShaderBaseStruct_ES2LoadProgram;
    int (*attrib)(unsigned int, const char *) = glGetAttribLocation;
    int (*uniform)(unsigned int, const char *) = glGetUniformLocation;
    void (*useProgram)(unsigned int) = glUseProgram;
    void (*uniform1i)(int, int) = glUniform1i;

    const char *vertex = "BloomShader.vsh";
    const char *positionName = "a_position";
    const char *texCoordName = "a_texCoord";
    const char *matrixName = "u_WorldMatrix";
    const char *samplerName = "s_texture";
    const char *texSizeName = "texSize";

    *(unsigned int *)((char *)this + 0x4) =
        loadProgram((ShaderBaseStruct *)this, vertex, "BloomShaderLuma.fsh");
    *(unsigned int *)((char *)this + 0x20) =
        loadProgram((ShaderBaseStruct *)this, vertex, "BloomShaderDownSample.fsh");
    *(unsigned int *)((char *)this + 0x38) =
        loadProgram((ShaderBaseStruct *)this, vertex, "BloomShaderBlurH.fsh");
    *(unsigned int *)((char *)this + 0x54) =
        loadProgram((ShaderBaseStruct *)this, vertex, "BloomShaderBlurV.fsh");
    *(unsigned int *)((char *)this + 0x74) =
        loadProgram((ShaderBaseStruct *)this, vertex, "BloomShaderFinal.fsh");

    *(int *)((char *)this + 0x8c) = attrib(*(unsigned int *)((char *)this + 0x4), positionName);
    *(int *)((char *)this + 0x94) = attrib(*(unsigned int *)((char *)this + 0x4), texCoordName);
    *(int *)((char *)this + 0x90) = uniform(*(unsigned int *)((char *)this + 0x4), matrixName);
    *(int *)((char *)this + 0x98) = uniform(*(unsigned int *)((char *)this + 0x4), samplerName);
    useProgram(*(unsigned int *)((char *)this + 0x4));
    uniform1i(*(int *)((char *)this + 0x98), 0);

    *(int *)((char *)this + 0x24) = attrib(*(unsigned int *)((char *)this + 0x20), positionName);
    *(int *)((char *)this + 0x2c) = attrib(*(unsigned int *)((char *)this + 0x20), texCoordName);
    *(int *)((char *)this + 0x28) = uniform(*(unsigned int *)((char *)this + 0x20), matrixName);
    *(int *)((char *)this + 0x30) = uniform(*(unsigned int *)((char *)this + 0x20), samplerName);
    useProgram(*(unsigned int *)((char *)this + 0x20));
    uniform1i(*(int *)((char *)this + 0x30), 0);

    *(int *)((char *)this + 0x3c) = attrib(*(unsigned int *)((char *)this + 0x38), positionName);
    *(int *)((char *)this + 0x44) = attrib(*(unsigned int *)((char *)this + 0x38), texCoordName);
    *(int *)((char *)this + 0x40) = uniform(*(unsigned int *)((char *)this + 0x38), matrixName);
    *(int *)((char *)this + 0x48) = uniform(*(unsigned int *)((char *)this + 0x38), samplerName);
    *(int *)((char *)this + 0x4c) = uniform(*(unsigned int *)((char *)this + 0x38), texSizeName);
    useProgram(*(unsigned int *)((char *)this + 0x38));
    uniform1i(*(int *)((char *)this + 0x48), 0);

    *(int *)((char *)this + 0x58) = attrib(*(unsigned int *)((char *)this + 0x54), positionName);
    *(int *)((char *)this + 0x60) = attrib(*(unsigned int *)((char *)this + 0x54), texCoordName);
    *(int *)((char *)this + 0x5c) = uniform(*(unsigned int *)((char *)this + 0x54), matrixName);
    *(int *)((char *)this + 0x64) = uniform(*(unsigned int *)((char *)this + 0x54), samplerName);
    *(int *)((char *)this + 0x68) = uniform(*(unsigned int *)((char *)this + 0x54), texSizeName);
    useProgram(*(unsigned int *)((char *)this + 0x54));
    uniform1i(*(int *)((char *)this + 0x64), 0);

    *(int *)((char *)this + 0x78) = attrib(*(unsigned int *)((char *)this + 0x74), positionName);
    *(int *)((char *)this + 0x80) = attrib(*(unsigned int *)((char *)this + 0x74), texCoordName);
    *(int *)((char *)this + 0x7c) = uniform(*(unsigned int *)((char *)this + 0x74), matrixName);
    *(int *)((char *)this + 0x84) = uniform(*(unsigned int *)((char *)this + 0x74), samplerName);
    *(int *)((char *)this + 0x88) =
        uniform(*(unsigned int *)((char *)this + 0x74), "s_texture_bloom");
    useProgram(*(unsigned int *)((char *)this + 0x74));
    uniform1i(*(int *)((char *)this + 0x84), 0);
    return uniform1i(*(int *)((char *)this + 0x88), 1);
}

} // namespace AbyssEngine

// ---- _BloomShader_8947c.cpp ----
extern "C" void _ZN11AbyssEngine11BloomShaderD0Ev(BloomShader *self)
{
    return operator_delete(_ZN11AbyssEngine11BloomShaderD1Ev(self));
}

// ---- UpdateMeshData_8a158.cpp ----
extern "C" void glUniformMatrix4fv(int location, int count, unsigned char transpose,
                                   const void *value);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type,
                                      unsigned char normalized, int stride, const void *pointer);

namespace AbyssEngine {

void BloomShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(*(int *)((char *)this + 0x90), 1, 0, (char *)engine + 0x104);
    if (*(uint8_t *)((char *)this + 0x9) != 0) {
        *(uint8_t *)((char *)this + 0x9) = 0;
    }

    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x8c));
    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x94));

    unsigned int zero = 0;
    if (*(uint8_t *)((char *)mesh + 0x5c) != 0) {
        glBindBuffer(0x8892, *(unsigned int *)((char *)mesh + 0x60));
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x8c), 3, 0x1406, 0, 0,
                              (void *)zero);
        glBindBuffer(0x8892, *(unsigned int *)((char *)mesh + 0x68));
        return glVertexAttribPointer(*(unsigned int *)((char *)this + 0x94), 2, 0x1406, 0, 0,
                                     (void *)zero);
    } else {
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x8c), 3, 0x1406, 0, 0,
                              *(void **)((char *)mesh + 0x4));
        return glVertexAttribPointer(*(unsigned int *)((char *)this + 0x94), 2, 0x1406, 0, 0,
                                     *(void **)((char *)mesh + 0x8));
    }
}

} // namespace AbyssEngine

// ---- RenderEffect_89820.cpp ----
extern "C" void glUniformMatrix4fv(int location, int count, unsigned char transpose,
                                   const void *value);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type,
                                      unsigned char normalized, int stride, const void *pointer);
extern "C" void Engine_DrawQuad(Engine *self, int x, int y, unsigned int width,
                                 unsigned int height);


namespace AbyssEngine {

void BloomShader::RenderEffect(FBOContainer *source, Engine *engine)
{
    void *volatile cookie = __stack_chk_guard;
    *(unsigned int *)((char *)engine + 0x3e4) = *(unsigned int *)((char *)this + 0x4);

    if (g_BloomShader_internalInitNeeded != 0) {
        g_BloomShader_internalInitNeeded = 0;
        InternalInit(engine);
        FBOContainer_BeginCapture(*(FBOContainer **)((char *)this + 0x70));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(0x4000);
        FBOContainer_EndCapture(*(FBOContainer **)((char *)this + 0x70));
    }

    typedef unsigned int u32x4 __attribute__((vector_size(16), aligned(4)));
    u32x4 zero = {0, 0, 0, 0};
    *(u32x4 *)((char *)engine + 0x3b4) = zero;
    *(u32x4 *)((char *)engine + 0x3a4) = zero;
    *(u32x4 *)((char *)engine + 0x394) = zero;
    *(u32x4 *)((char *)engine + 0x384) = zero;
    float matrix[16] = {};
    matrix[0] = 1.0f;
    matrix[5] = 1.0f;
    matrix[10] = 1.0f;
    matrix[15] = 1.0f;
    *(float *)((char *)engine + 0x384) = 2.0f / (float)Engine_GetDisplayWidth(engine);
    *(float *)((char *)engine + 0x398) = -(2.0f / (float)Engine_GetDisplayHeight(engine));
    *(unsigned int *)((char *)engine + 0x3ac) = 0xbf800000;
    *(unsigned int *)((char *)engine + 0x3b4) = 0xbf800000;
    *(unsigned int *)((char *)engine + 0x3b8) = 0x3f800000;
    *(unsigned int *)((char *)engine + 0x3c0) = 0x3f800000;
    Engine_SetWorldViewMatrix(engine);

    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);

    glUseProgram(*(unsigned int *)((char *)this + 0x20));
    glActiveTexture(0x84c0);
    FBOContainer_Activate(source);
    FBOContainer_BeginCapture(*(FBOContainer **)((char *)this + 0x34));
    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x24));
    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x2c));
    const void *mvp = (char *)engine + 0x104;
    glUniformMatrix4fv(*(int *)((char *)this + 0x28), 1, 0, mvp);
    glVertexAttribPointer(*(unsigned int *)((char *)this + 0x24), 3, 0x1406, 0, 0,
                          *(void **)(*(char **)((char *)engine + 0x380) + 4));
    glVertexAttribPointer(*(unsigned int *)((char *)this + 0x2c), 2, 0x1406, 0, 0,
                          *(void **)(*(char **)((char *)engine + 0x380) + 8));
    glClear(0x4000);
    Engine_DrawQuad(engine, 0, 0, Engine_GetDisplayWidth(engine), Engine_GetDisplayHeight(engine));
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x24));
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x2c));

    FBOContainer *blurSource = *(FBOContainer **)((char *)this + 0x34);
    for (int i = 6; i != 0; i -= 1) {
        glUseProgram(*(unsigned int *)((char *)this + 0x38));
        glActiveTexture(0x84c0);
        FBOContainer_Activate(blurSource);
        FBOContainer_BeginCapture(*(FBOContainer **)((char *)this + 0x50));
        glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x3c));
        glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x44));
        glUniformMatrix4fv(*(int *)((char *)this + 0x40), 1, 0, mvp);
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x3c), 3, 0x1406, 0, 0,
                              *(void **)(*(char **)((char *)engine + 0x380) + 4));
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x44), 2, 0x1406, 0, 0,
                              *(void **)(*(char **)((char *)engine + 0x380) + 8));
        glUniform1f(*(int *)((char *)this + 0x4c),
                    (float)*(int *)((char *)*(FBOContainer **)((char *)this + 0x50) + 0xc));
        glClear(0x4000);
        Engine_DrawQuad(engine, 0, 0, Engine_GetDisplayWidth(engine),
                        Engine_GetDisplayHeight(engine));
        glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x3c));
        glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x44));

        glUseProgram(*(unsigned int *)((char *)this + 0x54));
        glActiveTexture(0x84c0);
        FBOContainer_Activate(*(FBOContainer **)((char *)this + 0x50));
        FBOContainer_BeginCapture(*(FBOContainer **)((char *)this + 0x6c));
        glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x58));
        glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x60));
        glUniformMatrix4fv(*(int *)((char *)this + 0x5c), 1, 0, mvp);
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x58), 3, 0x1406, 0, 0,
                              *(void **)(*(char **)((char *)engine + 0x380) + 4));
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x60), 2, 0x1406, 0, 0,
                              *(void **)(*(char **)((char *)engine + 0x380) + 8));
        glUniform1f(*(int *)((char *)this + 0x68),
                    (float)*(int *)((char *)*(FBOContainer **)((char *)this + 0x6c) + 0x10));
        glClear(0x4000);
        Engine_DrawQuad(engine, 0, 0, Engine_GetDisplayWidth(engine),
                        Engine_GetDisplayHeight(engine));
        glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x58));
        glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x60));
        blurSource = *(FBOContainer **)((char *)this + 0x6c);
    }

    FBOContainer *base = source;
    FBOContainer *bloom = *(FBOContainer **)((char *)this + 0x70);
    if (g_BloomShader_shaderMode < 4) {
        switch (g_BloomShader_shaderMode) {
        case 1:
            base = *(FBOContainer **)((char *)this + 0x34);
            break;
        case 2:
            base = *(FBOContainer **)((char *)this + 0x6c);
            break;
        case 3:
            bloom = *(FBOContainer **)((char *)this + 0x70);
            break;
        default:
            break;
        }
    }

    glUseProgram(*(unsigned int *)((char *)this + 0x74));
    glActiveTexture(0x84c0);
    FBOContainer_Activate(base);
    glActiveTexture(0x84c1);
    FBOContainer_Activate(bloom);
    glBindFramebuffer(0x8d40, *(unsigned int *)((char *)engine + 0x40c));
    unsigned int width;
    unsigned int height;
    if (*(int *)(**(char ***)((char *)engine + 0x30) + 0x30) == 2) {
        width = Engine_GetDisplayWidth(engine);
        height = Engine_GetDisplayHeight(engine);
    } else {
        width = Engine_GetDisplayHeight(engine);
        height = Engine_GetDisplayWidth(engine);
    }
    glViewport(0, 0, width, height);

    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x78));
    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x80));
    glUniformMatrix4fv(*(int *)((char *)this + 0x7c), 1, 0, mvp);
    glVertexAttribPointer(*(unsigned int *)((char *)this + 0x78), 3, 0x1406, 0, 0,
                          *(void **)(*(char **)((char *)engine + 0x380) + 4));
    glVertexAttribPointer(*(unsigned int *)((char *)this + 0x80), 2, 0x1406, 0, 0,
                          *(void **)(*(char **)((char *)engine + 0x380) + 8));
    glClear(0x4000);
    Engine_DrawQuad(engine, 0, 0, Engine_GetDisplayWidth(engine), Engine_GetDisplayHeight(engine));
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x78));
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x80));
    glEnable(0xbe2);
    glBlendFunc(0x302, 0x303);
    glActiveTexture(0x84c0);

    unsigned int guardDelta = (unsigned int)__stack_chk_guard - (unsigned int)cookie;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine

// ---- SetInActive_897f4.cpp ----
namespace AbyssEngine {

void BloomShader::SetInActive()
{
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x8c));
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x94));
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x2c));
    return glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x24));
}

} // namespace AbyssEngine

// ---- BloomShader_893dc.cpp ----
namespace AbyssEngine {

__attribute__((minsize)) BloomShader::BloomShader()
{
    void *volatile cookie = __stack_chk_guard;
    ShaderBaseStruct_ctor((ShaderBaseStruct *)this);
    *(void **)this = &BloomShader_vtable + 8;
    *(void **)BloomShader_typeinfo_dest = *(void **)BloomShader_typeinfo_source;
    String name("BloomShader", false);
    *(String *)((char *)this + 0xc) = name;
    name.~String();
    unsigned int guardDelta = (unsigned int)__stack_chk_guard - (unsigned int)cookie;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine

// ---- InternalInit_8948c.cpp ----
void *operator new(__SIZE_TYPE__ size);

extern "C" void FBOContainer_Create(FBOContainer *self, unsigned int width, unsigned int height,
                                    bool depth, bool stencil);

namespace AbyssEngine {

void BloomShader::InternalInit(Engine *engine)
{
    void *volatile cookie = __stack_chk_guard;

    FBOContainer *fbo = (FBOContainer *)operator new(0x38);
    String luma("BloomShader fboLuma", false);
    FBOContainer_ctor(fbo, engine, &luma);
    *(FBOContainer **)((char *)this + 0x34) = fbo;
    luma.~String();
    FBOContainer_Create(*(FBOContainer **)((char *)this + 0x34), 0x100, 0x100, true, false);

    fbo = (FBOContainer *)operator new(0x38);
    String blurH("BloomShader fboBlurH", false);
    FBOContainer_ctor(fbo, engine, &blurH);
    *(FBOContainer **)((char *)this + 0x50) = fbo;
    blurH.~String();
    FBOContainer_Create(*(FBOContainer **)((char *)this + 0x50), 0x100, 0x100, true, false);

    fbo = (FBOContainer *)operator new(0x38);
    String blurV("BloomShader fboBlurV", false);
    FBOContainer_ctor(fbo, engine, &blurV);
    *(FBOContainer **)((char *)this + 0x6c) = fbo;
    blurV.~String();
    FBOContainer_Create(*(FBOContainer **)((char *)this + 0x6c), 0x100, 0x100, true, false);

    fbo = (FBOContainer *)operator new(0x38);
    String black("BloomShader fboBlack", false);
    FBOContainer_ctor(fbo, engine, &black);
    *(FBOContainer **)((char *)this + 0x70) = fbo;
    black.~String();
    FBOContainer_Create(*(FBOContainer **)((char *)this + 0x70), 0x100, 0x100, true, false);

    unsigned int guardDelta = (unsigned int)__stack_chk_guard - (unsigned int)cookie;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine

// ---- _BloomShader_89468.cpp ----
extern "C" void *_ZN11AbyssEngine11BloomShaderD1Ev(BloomShader *self)
{
    *(void **)self = &BloomShader_vtable + 8;
    return ShaderBaseStruct_dtor((ShaderBaseStruct *)self);
}

// ---- RenderEffect_89cd8.cpp ----
extern "C" void glUniformMatrix4fv(int location, int count, unsigned char transpose,
                                   const void *value);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type,
                                      unsigned char normalized, int stride, const void *pointer);
extern "C" void Engine_DrawQuad(Engine *self, int x, int y, unsigned int width,
                                 unsigned int height);


namespace AbyssEngine {

void BloomShader::RenderEffect(FBOContainer *source, FBOContainer **target, Engine *engine)
{
    void *volatile cookie = __stack_chk_guard;

    if (g_BloomShader_internalInitNeeded != 0) {
        g_BloomShader_internalInitNeeded = 0;
        InternalInit(engine);
        FBOContainer_BeginCapture(*(FBOContainer **)((char *)this + 0x70));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(0x4000);
        FBOContainer_EndCapture(*(FBOContainer **)((char *)this + 0x70));
    }

    typedef unsigned int u32x4 __attribute__((vector_size(16), aligned(4)));
    u32x4 zero = {0, 0, 0, 0};
    *(u32x4 *)((char *)engine + 0x3b4) = zero;
    *(u32x4 *)((char *)engine + 0x3a4) = zero;
    *(u32x4 *)((char *)engine + 0x394) = zero;
    *(u32x4 *)((char *)engine + 0x384) = zero;
    float matrix[16] = {};
    matrix[0] = 1.0f;
    matrix[5] = 1.0f;
    matrix[10] = 1.0f;
    matrix[15] = 1.0f;
    *(float *)((char *)engine + 0x384) = 2.0f / (float)Engine_GetDisplayWidth(engine);
    *(float *)((char *)engine + 0x398) = -(2.0f / (float)Engine_GetDisplayHeight(engine));
    *(unsigned int *)((char *)engine + 0x3ac) = 0xbf800000;
    *(unsigned int *)((char *)engine + 0x3b4) = 0xbf800000;
    *(unsigned int *)((char *)engine + 0x3b8) = 0x3f800000;
    *(unsigned int *)((char *)engine + 0x3c0) = 0x3f800000;
    Engine_SetWorldViewMatrix(engine);

    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);

    glUseProgram(*(unsigned int *)((char *)this + 0x20));
    glActiveTexture(0x84c0);
    FBOContainer_Activate(source);
    FBOContainer_BeginCapture(*(FBOContainer **)((char *)this + 0x34));
    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x24));
    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x2c));
    const void *mvp = (char *)engine + 0x104;
    glUniformMatrix4fv(*(int *)((char *)this + 0x28), 1, 0, mvp);
    glVertexAttribPointer(*(unsigned int *)((char *)this + 0x24), 3, 0x1406, 0, 0,
                          *(void **)(*(char **)((char *)engine + 0x380) + 4));
    glVertexAttribPointer(*(unsigned int *)((char *)this + 0x2c), 2, 0x1406, 0, 0,
                          *(void **)(*(char **)((char *)engine + 0x380) + 8));
    glClear(0x4000);
    Engine_DrawQuad(engine, 0, 0, Engine_GetDisplayWidth(engine), Engine_GetDisplayHeight(engine));
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x24));
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x2c));

    FBOContainer *blurSource = *(FBOContainer **)((char *)this + 0x34);
    for (int i = 6; i != 0; i -= 1) {
        glUseProgram(*(unsigned int *)((char *)this + 0x38));
        glActiveTexture(0x84c0);
        FBOContainer_Activate(blurSource);
        FBOContainer_BeginCapture(*(FBOContainer **)((char *)this + 0x50));
        glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x3c));
        glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x44));
        glUniformMatrix4fv(*(int *)((char *)this + 0x40), 1, 0, mvp);
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x3c), 3, 0x1406, 0, 0,
                              *(void **)(*(char **)((char *)engine + 0x380) + 4));
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x44), 2, 0x1406, 0, 0,
                              *(void **)(*(char **)((char *)engine + 0x380) + 8));
        glUniform1f(*(int *)((char *)this + 0x4c),
                    (float)*(int *)((char *)*(FBOContainer **)((char *)this + 0x50) + 0xc));
        glClear(0x4000);
        Engine_DrawQuad(engine, 0, 0, Engine_GetDisplayWidth(engine),
                        Engine_GetDisplayHeight(engine));
        glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x3c));
        glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x44));

        glUseProgram(*(unsigned int *)((char *)this + 0x54));
        glActiveTexture(0x84c0);
        FBOContainer_Activate(*(FBOContainer **)((char *)this + 0x50));
        FBOContainer_BeginCapture(*(FBOContainer **)((char *)this + 0x6c));
        glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x58));
        glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x60));
        glUniformMatrix4fv(*(int *)((char *)this + 0x5c), 1, 0, mvp);
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x58), 3, 0x1406, 0, 0,
                              *(void **)(*(char **)((char *)engine + 0x380) + 4));
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x60), 2, 0x1406, 0, 0,
                              *(void **)(*(char **)((char *)engine + 0x380) + 8));
        glUniform1f(*(int *)((char *)this + 0x68),
                    (float)*(int *)((char *)*(FBOContainer **)((char *)this + 0x6c) + 0x10));
        glClear(0x4000);
        Engine_DrawQuad(engine, 0, 0, Engine_GetDisplayWidth(engine),
                        Engine_GetDisplayHeight(engine));
        glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x58));
        glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x60));
        blurSource = *(FBOContainer **)((char *)this + 0x6c);
    }

    FBOContainer *base = source;
    FBOContainer *bloom = *(FBOContainer **)((char *)this + 0x70);
    if (g_BloomShader_shaderMode < 4) {
        switch (g_BloomShader_shaderMode) {
        case 1:
            base = *(FBOContainer **)((char *)this + 0x34);
            break;
        case 2:
            base = *(FBOContainer **)((char *)this + 0x6c);
            break;
        case 3:
            bloom = *(FBOContainer **)((char *)this + 0x70);
            break;
        default:
            break;
        }
    }

    glUseProgram(*(unsigned int *)((char *)this + 0x74));
    glActiveTexture(0x84c0);
    FBOContainer_Activate(base);
    glActiveTexture(0x84c1);
    FBOContainer_Activate(bloom);
    if (*target != 0) {
        FBOContainer_BeginCapture(*target);
    }

    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x78));
    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x80));
    glUniformMatrix4fv(*(int *)((char *)this + 0x7c), 1, 0, mvp);
    glVertexAttribPointer(*(unsigned int *)((char *)this + 0x78), 3, 0x1406, 0, 0,
                          *(void **)(*(char **)((char *)engine + 0x380) + 4));
    glVertexAttribPointer(*(unsigned int *)((char *)this + 0x80), 2, 0x1406, 0, 0,
                          *(void **)(*(char **)((char *)engine + 0x380) + 8));
    glClear(0x4000);
    Engine_DrawQuad(engine, 0, 0, Engine_GetDisplayWidth(engine), Engine_GetDisplayHeight(engine));
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x78));
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x80));
    glEnable(0xbe2);
    glBlendFunc(0x302, 0x303);
    glActiveTexture(0x84c0);
    if (*target != 0) {
        FBOContainer_EndCapture(*target);
    }

    unsigned int guardDelta = (unsigned int)__stack_chk_guard - (unsigned int)cookie;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
