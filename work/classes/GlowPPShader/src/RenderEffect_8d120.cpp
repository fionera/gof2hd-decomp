#include "class.h"

extern "C" void GlowPPShader_InternalInit(GlowPPShader *self, Engine *engine);
extern "C" uint8_t *g_GlowPPShader_internalInitNeededPtr;
extern "C" uint32_t *g_GlowPPShader_shaderModePtr;

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
    uint32_t width = Engine_GetDisplayWidth(engine);
    Engine_GetDisplayHeight(engine);
    Engine_DrawQuad(engine, 0, 0, width);
    glDisableVertexAttribArray(field_u32(self, posOff));
    glDisableVertexAttribArray(field_u32(self, texOff));
}

extern "C" void GlowPPShader_RenderEffect(GlowPPShader *self, FBOContainer *source,
                                          FBOContainer **target, Engine *engine)
{
    void * volatile cookie = __stack_chk_guard;

    if (*g_GlowPPShader_internalInitNeededPtr != 0) {
        *g_GlowPPShader_internalInitNeededPtr = 0;
        GlowPPShader_InternalInit(self, engine);
        FBOContainer_BeginCapture(fbo_field(self, 0xa0));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(0x4000);
        FBOContainer_EndCapture(fbo_field(self, 0xa0));
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

    field_float(engine, 0x384) = 2.0f / (float)(int32_t)Engine_GetDisplayWidth(engine);
    field_float(engine, 0x398) = -(2.0f / (float)(int32_t)Engine_GetDisplayHeight(engine));
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
    (void)matrix;

    Engine_SetWorldViewMatrix(engine);
    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);

    glUseProgram(field_u32(self, 0x20));
    glActiveTexture(0x84c0);
    FBOContainer_Activate(source);
    FBOContainer_BeginCapture(fbo_field(self, 0x3c));
    draw_fullscreen(self, engine, 0x24, 0x2c, 0x28);

    for (int32_t i = 3; i != 0; --i) {
        glUseProgram(field_u32(self, 0x40));
        glActiveTexture(0x84c0);
        FBOContainer_Activate(fbo_field(self, 0x3c));
        FBOContainer_BeginCapture(fbo_field(self, 0x58));
        glEnableVertexAttribArray(field_u32(self, 0x44));
        glEnableVertexAttribArray(field_u32(self, 0x4c));
        glUniformMatrix4fv(field_u32(self, 0x48), 1, 0, (char *)engine + 0x104);
        glVertexAttribPointer(field_u32(self, 0x44), 3, 0x1406, 0, 0,
                              (void *)field_u32((void *)field_u32(engine, 0x380), 4));
        glVertexAttribPointer(field_u32(self, 0x4c), 2, 0x1406, 0, 0,
                              (void *)field_u32((void *)field_u32(engine, 0x380), 8));
        glUniform1f(field_u32(self, 0x54),
                    1.0f / (float)field_i32((void *)field_u32(self, 0x58), 0x0c));
        glClear(0x4000);
        uint32_t width = Engine_GetDisplayWidth(engine);
        Engine_GetDisplayHeight(engine);
        Engine_DrawQuad(engine, 0, 0, width);
        glDisableVertexAttribArray(field_u32(self, 0x44));
        glDisableVertexAttribArray(field_u32(self, 0x4c));

        glUseProgram(field_u32(self, 0x5c));
        glActiveTexture(0x84c0);
        FBOContainer_Activate(fbo_field(self, 0x58));
        FBOContainer_BeginCapture(fbo_field(self, 0x74));
        glEnableVertexAttribArray(field_u32(self, 0x60));
        glEnableVertexAttribArray(field_u32(self, 0x68));
        glUniformMatrix4fv(field_u32(self, 0x64), 1, 0, (char *)engine + 0x104);
        glVertexAttribPointer(field_u32(self, 0x60), 3, 0x1406, 0, 0,
                              (void *)field_u32((void *)field_u32(engine, 0x380), 4));
        glVertexAttribPointer(field_u32(self, 0x68), 2, 0x1406, 0, 0,
                              (void *)field_u32((void *)field_u32(engine, 0x380), 8));
        glUniform1f(field_u32(self, 0x70),
                    1.0f / (float)field_i32((void *)field_u32(self, 0x74), 0x10));
        glClear(0x4000);
        width = Engine_GetDisplayWidth(engine);
        Engine_GetDisplayHeight(engine);
        Engine_DrawQuad(engine, 0, 0, width);
        glDisableVertexAttribArray(field_u32(self, 0x60));
        glDisableVertexAttribArray(field_u32(self, 0x68));
    }

    FBOContainer *firstTexture = fbo_field(self, 0x74);
    FBOContainer *secondTexture = fbo_field(self, 0xa0);
    uint32_t mode = *g_GlowPPShader_shaderModePtr;
    if (mode == 0) {
        firstTexture = fbo_field(self, 0x3c);
    } else if (mode == 1) {
        firstTexture = source;
    }

    glUseProgram(field_u32(self, 0x78));
    glActiveTexture(0x84c0);
    FBOContainer_Activate(firstTexture);
    glActiveTexture(0x84c1);
    FBOContainer_Activate(secondTexture);

    if (*target == 0) {
        glBindFramebuffer(0x8d40, field_u32(engine, 0x40c));
        uint32_t width;
        uint32_t height;
        if (field_i32((void *)field_u32((void *)field_u32(engine, 0x30), 0), 0x30) == 2) {
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

    draw_fullscreen(self, engine, 0x7c, 0x84, 0x80);
    if (*target != 0) {
        FBOContainer_EndCapture(*target);
    }

    glEnable(0xbe2);
    glBlendFunc(0x302, 0x303);
    glActiveTexture(0x84c0);

    uint32_t diff = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (diff == 0) {
        return;
    }
    __stack_chk_fail(diff);
}
