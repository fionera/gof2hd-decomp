#include "class.h"
#include <arm_neon.h>

namespace AbyssEngine {

typedef uint64_t unaligned_u64 __attribute__((aligned(1)));

void DrawFBOShader::RenderEffect(FBOContainer *fbo, Engine *engine)
{
    void * volatile cookie = __stack_chk_guard;
    char *self = (char *)this;
    char *engineBytes = (char *)engine;

    uint32x4_t zero = vdupq_n_u32(0);
    vst1q_u32((uint32_t *)(engineBytes + 0x3b4), zero);
    vst1q_u32((uint32_t *)(engineBytes + 0x3a4), zero);
    vst1q_u32((uint32_t *)(engineBytes + 0x394), zero);
    vst1q_u32((uint32_t *)(engineBytes + 0x384), zero);

    float two = 2.0f;
    *(float *)(engineBytes + 0x384) = two / (float)engine->GetDisplayWidth();
    int32_t height = engine->GetDisplayHeight();

    uint64x2_t tail = {0x000000003f800000ULL, 0x3f8000003f800000ULL};

    uint32_t one = 0x3f800000;
    *(uint32_t *)(engineBytes + 0x3ac) = 0xbd4ccccd;
    *(uint32_t *)(engineBytes + 0x3c0) = one;
    *(uint32_t *)(engineBytes + 0x3b4) = 0xbf800000;
    *(uint32_t *)(engineBytes + 0x3b8) = one;
    *(float *)(engineBytes + 0x398) = -(two / (float)height);

    AEMath::Matrix matrix;
    vst1q_u32((uint32_t *)((uint32_t)matrix.m | 4), zero);
    vst1q_u64((unaligned_u64 *)(matrix.m + 6), vreinterpretq_u64_u32(zero));
    vst1q_u64((unaligned_u64 *)(matrix.m + 10), tail);
    volatile uint32_t *matrixWords = matrix.m;
    matrixWords[0] = one;
    matrixWords[5] = one;
    matrixWords[14] = one;

    engine->SetWorldViewMatrix(matrix);
    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);
    glUseProgram(*(uint32_t *)(self + 0x4));
    glActiveTexture(0x84c0);
    fbo->Activate();

    if (engine->IsPostEffectActivated() == 0) {
        glBindFramebuffer(0x8d40, *(uint32_t *)(engineBytes + 0x40c));
        int width;
        int viewportHeight;
        if (*(int32_t *)(*(char **)(*(char **)(engineBytes + 0x30)) + 0x30) == 2) {
            width = engine->GetDisplayWidth();
            viewportHeight = engine->GetDisplayHeight();
        } else {
            width = engine->GetDisplayHeight();
            viewportHeight = engine->GetDisplayWidth();
        }
        glViewport(0, 0, width, viewportHeight);
    } else {
        engine->ActivateRender2FracFBO();
    }

    glEnableVertexAttribArray(*(uint32_t *)(self + 0x48));
    glEnableVertexAttribArray(*(uint32_t *)(self + 0x50));
    glUniformMatrix4fv(*(int32_t *)(self + 0x4c), 1, 0, engineBytes + 0x104);

    char *mesh = *(char **)(engineBytes + 0x380);
    glVertexAttribPointer(*(uint32_t *)(self + 0x48), 3, 0x1406, 0, 0,
                          *(void **)(mesh + 0x4));
    mesh = *(char **)(engineBytes + 0x380);
    glVertexAttribPointer(*(uint32_t *)(self + 0x50), 2, 0x1406, 0, 0,
                          *(void **)(mesh + 0x8));

    void (*clear)(uint32_t) = glClear;
    clear(0x4000);
    clear(0x100);

    int drawWidth = engine->GetDisplayWidth();
    int drawHeight = engine->GetDisplayHeight();
    engine->DrawQuad(0, 0, drawWidth, drawHeight);

    glDisableVertexAttribArray(*(uint32_t *)(self + 0x48));
    glDisableVertexAttribArray(*(uint32_t *)(self + 0x50));
    glEnable(0xb71);
    clear(0x100);

    if (engine->IsPostEffectActivated() != 0) {
        engine->DeactivateRender2FracFBO();
    }
    glActiveTexture(0x84c0);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
