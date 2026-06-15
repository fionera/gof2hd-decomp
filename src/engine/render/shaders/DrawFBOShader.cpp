#include "gof2/engine/render/shaders/DrawFBOShader.h"
#include "gof2/engine/render/FBOContainer.h"
#include "gof2/engine/render/Engine.h"
#include "gof2/engine/render/Mesh.h"
#include "gof2/platform/gl.h"
#include <arm_neon.h>

// Cross-object reads of Engine/Mesh are deferred to a later Engine/Mesh-modeling pass; until
// then their fields are reached by raw offset.
template <class T> static inline T &ae_field(void *base, unsigned int off) {
    return *(T *)((char *)base + off);
}

// DrawFBOShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char DrawFBOShader_vtable;

// Engine entry points reached by opaque pointer (modelled in a later pass).
extern "C" unsigned int Engine_GetDisplayWidth(::Engine *engine);
extern "C" unsigned int Engine_GetDisplayHeight(::Engine *engine);
extern "C" void Engine_DrawQuad(::Engine *engine, int x, int y, int width, int height);
extern "C" void Engine_SetWorldViewMatrix(::Engine *engine, const uint32_t *matrix);
extern "C" int Engine_IsPostEffectActivated(::Engine *engine);
extern "C" void Engine_ActivateRender2FracFBO(::Engine *engine);
extern "C" void Engine_DeactivateRender2FracFBO(::Engine *engine);

namespace AbyssEngine {

int DrawFBOShader::ShaderIndex;

DrawFBOShader::DrawFBOShader()
{
    this->vtable = &DrawFBOShader_vtable + 8;
    this->name.s = u"DrawFBOShader";
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
}

// Compiles the full-screen blit program and caches its attribute/uniform locations.
void DrawFBOShader::Init(Engine *)
{
    this->program = this->ES2LoadProgram("DrawFBOShader.vsh", "DrawFBOShader.fsh");

    this->positionLoc = glGetAttribLocation(this->program, "position");
    this->texCoordLoc = glGetAttribLocation(this->program, "texCoord");
    this->worldViewMatrixLoc = glGetUniformLocation(this->program, "worldViewMatrix");
    this->textureLoc = glGetUniformLocation(this->program, "texture");

    glUseProgram(this->program);
    glUniform1i(this->textureLoc, 0);
}

// Binds the world matrix and vertex/texcoord arrays for the supplied mesh.
void DrawFBOShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(this->worldViewMatrixLoc, 1, 0, engine->worldViewProjMatrix);
    if (this->dirty != 0) {
        this->dirty = 0;
    }

    glEnableVertexAttribArray(this->positionLoc);
    glEnableVertexAttribArray(this->texCoordLoc);

    if (mesh->uploaded == 0) {
        glVertexAttribPointer(this->positionLoc, 3, 0x1406, 0, 0, mesh->positions);
        glVertexAttribPointer(this->texCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
    } else {
        glBindBuffer(0x8892, mesh->positionVBO);
        glVertexAttribPointer(this->positionLoc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->texCoordVBO);
        glVertexAttribPointer(this->texCoordLoc, 2, 0x1406, 0, 0, 0);
    }
}

typedef uint64_t unaligned_u64 __attribute__((aligned(1)));

// Blits the source FBO across the full screen (orthographic quad), optionally through the
// post-effect / fractional render targets.
void DrawFBOShader::RenderEffect(FBOContainer *fbo, Engine *engine)
{
    char *engineBytes = (char *)engine;

    uint32x4_t zero = vdupq_n_u32(0);
    vst1q_u32((uint32_t *)(engineBytes + 0x3b4), zero);
    vst1q_u32((uint32_t *)(engineBytes + 0x3a4), zero);
    vst1q_u32((uint32_t *)(engineBytes + 0x394), zero);
    vst1q_u32((uint32_t *)(engineBytes + 0x384), zero);

    float two = 2.0f;
    *(float *)(engineBytes + 0x384) = two / (float)Engine_GetDisplayWidth(engine);
    unsigned int height = Engine_GetDisplayHeight(engine);

    uint64x2_t tail = {0x000000003f800000ULL, 0x3f8000003f800000ULL};

    uint32_t one = 0x3f800000;
    *(uint32_t *)(engineBytes + 0x3ac) = 0xbd4ccccd;
    *(uint32_t *)(engineBytes + 0x3c0) = one;
    *(uint32_t *)(engineBytes + 0x3b4) = 0xbf800000;
    *(uint32_t *)(engineBytes + 0x3b8) = one;
    *(float *)(engineBytes + 0x398) = -(two / (float)height);

    uint32_t matrix[16];
    vst1q_u32(matrix + 4, zero);
    vst1q_u64((unaligned_u64 *)(matrix + 6), vreinterpretq_u64_u32(zero));
    vst1q_u64((unaligned_u64 *)(matrix + 10), tail);
    matrix[0] = one;
    matrix[5] = one;
    matrix[14] = one;

    Engine_SetWorldViewMatrix(engine, matrix);
    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);
    glUseProgram(this->program);
    glActiveTexture(0x84c0);
    fbo->Activate();

    if (Engine_IsPostEffectActivated(engine) == 0) {
        glBindFramebuffer(0x8d40, ae_field<unsigned int>(engine, 0x40c));
        unsigned int width;
        unsigned int viewportHeight;
        if (*(int *)((char *)engine->field_0x30 + 0x30) == 2) {
            width = Engine_GetDisplayWidth(engine);
            viewportHeight = Engine_GetDisplayHeight(engine);
        } else {
            width = Engine_GetDisplayHeight(engine);
            viewportHeight = Engine_GetDisplayWidth(engine);
        }
        glViewport(0, 0, width, viewportHeight);
    } else {
        Engine_ActivateRender2FracFBO(engine);
    }

    glEnableVertexAttribArray(this->positionLoc);
    glEnableVertexAttribArray(this->texCoordLoc);
    glUniformMatrix4fv(this->worldViewMatrixLoc, 1, 0, engine->worldViewProjMatrix);

    char *mesh = ae_field<char *>(engine, 0x380);
    glVertexAttribPointer(this->positionLoc, 3, 0x1406, 0, 0, *(void **)(mesh + 0x4));
    mesh = ae_field<char *>(engine, 0x380);
    glVertexAttribPointer(this->texCoordLoc, 2, 0x1406, 0, 0, *(void **)(mesh + 0x8));

    glClear(0x4000);
    glClear(0x100);

    int drawWidth = Engine_GetDisplayWidth(engine);
    int drawHeight = Engine_GetDisplayHeight(engine);
    Engine_DrawQuad(engine, 0, 0, drawWidth, drawHeight);

    glDisableVertexAttribArray(this->positionLoc);
    glDisableVertexAttribArray(this->texCoordLoc);
    glEnable(0xb71);
    glClear(0x100);

    if (Engine_IsPostEffectActivated(engine) != 0) {
        Engine_DeactivateRender2FracFBO(engine);
    }
    glActiveTexture(0x84c0);
}

void DrawFBOShader::SetInActive()
{
    glDisableVertexAttribArray(this->positionLoc);
    glDisableVertexAttribArray(this->texCoordLoc);
}

} // namespace AbyssEngine
