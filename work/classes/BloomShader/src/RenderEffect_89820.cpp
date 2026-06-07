#include "class.h"

extern "C" void glDisable(unsigned int cap);
extern "C" void glEnable(unsigned int cap);
extern "C" void glDepthMask(unsigned char flag);
extern "C" void glUseProgram(unsigned int program);
extern "C" void glActiveTexture(unsigned int texture);
extern "C" void glBindFramebuffer(unsigned int target, unsigned int framebuffer);
extern "C" void glViewport(int x, int y, unsigned int width, unsigned int height);
extern "C" void glEnableVertexAttribArray(unsigned int index);
extern "C" void glDisableVertexAttribArray(unsigned int index);
extern "C" void glUniformMatrix4fv(int location, int count, unsigned char transpose,
                                   const void *value);
extern "C" void glUniform1f(int location, float value);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type,
                                      unsigned char normalized, int stride, const void *pointer);
extern "C" void glClearColor(float red, float green, float blue, float alpha);
extern "C" void glClear(unsigned int mask);
extern "C" void glBlendFunc(unsigned int sfactor, unsigned int dfactor);
extern "C" int Engine_GetDisplayWidth(Engine *self);
extern "C" int Engine_GetDisplayHeight(Engine *self);
extern "C" void Engine_SetWorldViewMatrix(Engine *self);
extern "C" void Engine_DrawQuad(Engine *self, int x, int y, unsigned int width,
                                 unsigned int height);
extern "C" void FBOContainer_Activate(FBOContainer *self);
extern "C" void FBOContainer_BeginCapture(FBOContainer *self);
extern "C" void FBOContainer_EndCapture(FBOContainer *self);

extern "C" unsigned char g_BloomShader_internalInitNeeded;
extern "C" unsigned int g_BloomShader_shaderMode;

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
