#include "class.h"

extern "C" void glDisable(unsigned int cap);
extern "C" void glEnable(unsigned int cap);
extern "C" void glDepthMask(unsigned char flag);
extern "C" void glUseProgram(unsigned int program);
extern "C" void glActiveTexture(unsigned int texture);
extern "C" void glBindFramebuffer(unsigned int target, unsigned int framebuffer);
extern "C" void glViewport(int x, int y, int width, int height);
extern "C" void glEnableVertexAttribArray(unsigned int index);
extern "C" void glDisableVertexAttribArray(unsigned int index);
extern "C" void glUniformMatrix4fv(int location, int count, unsigned char transpose,
                                   const void *value);
extern "C" void glUniform2f(int location, float x, float y);
extern "C" void glUniform1f(int location, float value);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type,
                                      unsigned char normalized, int stride, const void *pointer);
extern "C" void glClear(unsigned int mask);
extern "C" void glBlendFunc(unsigned int sfactor, unsigned int dfactor);
extern "C" int Engine_GetDisplayWidth(Engine *self);
extern "C" int Engine_GetDisplayHeight(Engine *self);
extern "C" void Engine_SetWorldViewMatrix(Engine *self);
extern "C" void Engine_DrawQuad(Engine *self, int x, int y, int width, int height);
extern "C" void FBOContainer_Activate(FBOContainer *self);
extern "C" void FBOContainer_BeginCapture(FBOContainer *self);
extern "C" void FBOContainer_EndCapture(FBOContainer *self);

namespace AbyssEngine {

void BlurShader::RenderEffect(FBOContainer *fbo, FBOContainer **target, Engine *engine,
                              float amount, Vector vector)
{
    void *volatile cookie = __stack_chk_guard;
    float matrix[16] = {};
    matrix[0] = 1.0f;
    matrix[5] = 1.0f;
    matrix[10] = 1.0f;
    matrix[15] = 1.0f;

    *(float *)((char *)engine + 0x384) = 2.0f / (float)Engine_GetDisplayWidth(engine);
    *(float *)((char *)engine + 0x398) = -(2.0f / (float)Engine_GetDisplayHeight(engine));
    *(uint32_t *)((char *)engine + 0x3ac) = 0xbf800000;
    *(uint32_t *)((char *)engine + 0x3b4) = 0xbf800000;
    *(uint32_t *)((char *)engine + 0x3b8) = 0x3f800000;
    *(uint32_t *)((char *)engine + 0x3c0) = 0x3f800000;
    Engine_SetWorldViewMatrix(engine);

    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);
    glUseProgram(*(unsigned int *)((char *)this + 0x24));
    glActiveTexture(0x84c0);
    FBOContainer_Activate(fbo);

    if (*target == 0) {
        glBindFramebuffer(0x8d40, *(unsigned int *)((char *)engine + 0x40c));
        int width;
        int height;
        if (*(int *)(**(char ***)((char *)engine + 0x30) + 0x30) == 2) {
            width = Engine_GetDisplayWidth(engine);
            height = Engine_GetDisplayHeight(engine);
        } else {
            width = Engine_GetDisplayHeight(engine);
            height = Engine_GetDisplayWidth(engine);
        }
        glViewport(0, 0, width, height);
    } else {
        FBOContainer_BeginCapture(*target);
    }

    int position = *(int *)((char *)this + 0x28);
    if (position >= 0) {
        glEnableVertexAttribArray(position);
    }
    int texCoord = *(int *)((char *)this + 0x30);
    if (texCoord >= 0) {
        glEnableVertexAttribArray(texCoord);
    }
    int matrixLocation = *(int *)((char *)this + 0x2c);
    if (matrixLocation >= 0) {
        glUniformMatrix4fv(matrixLocation, 1, 0, (char *)engine + 0x104);
    }
    int texelLocation = *(int *)((char *)this + 0x38);
    if (texelLocation >= 0) {
        float width;
        int other;
        if (*(int *)(**(char ***)((char *)engine + 0x30) + 0x30) == 2) {
            width = (float)Engine_GetDisplayWidth(engine);
            other = Engine_GetDisplayHeight(engine);
        } else {
            width = (float)Engine_GetDisplayHeight(engine);
            other = Engine_GetDisplayWidth(engine);
        }
        glUniform2f(texelLocation, 1.0f / width, 1.0f / (float)other);
    }
    int centerLocation = *(int *)((char *)this + 0x44);
    if (centerLocation >= 0) {
        float x = vector[0];
        float y = vector[1];
        glUniform2f(centerLocation, x, y);
    }
    int strengthLocation = *(int *)((char *)this + 0x40);
    if (strengthLocation >= 0) {
        glUniform1f(strengthLocation, *(float *)((char *)this + 0x58));
    }
    int amountLocation = *(int *)((char *)this + 0x3c);
    if (amountLocation >= 0) {
        float scaled = amount;
        if (scaled < 0.2f) {
            scaled = 0.2f;
        }
        glUniform1f(amountLocation, scaled * *(float *)((char *)this + 0x5c));
    }
    if (position >= 0) {
        glVertexAttribPointer(position, 3, 0x1406, 0, 0,
                              *(void **)(*(char **)((char *)engine + 0x380) + 4));
    }
    if (texCoord >= 0) {
        glVertexAttribPointer(texCoord, 2, 0x1406, 0, 0,
                              *(void **)(*(char **)((char *)engine + 0x380) + 8));
    }

    glClear(0x4000);
    int width = Engine_GetDisplayWidth(engine);
    Engine_DrawQuad(engine, 0, 0, width, Engine_GetDisplayHeight(engine));
    if (position >= 0) {
        glDisableVertexAttribArray(position);
    }
    if (texCoord >= 0) {
        glDisableVertexAttribArray(texCoord);
    }
    if (*target != 0) {
        FBOContainer_EndCapture(*target);
    }
    glEnable(0xbe2);
    glBlendFunc(0x302, 0x303);
    glActiveTexture(0x84c0);
    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}

} // namespace AbyssEngine
