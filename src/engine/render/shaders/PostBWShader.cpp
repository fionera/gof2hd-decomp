#include "gof2/engine/render/shaders/PostBWShader.h"
#include "gof2/engine/render/FBOContainer.h"
#include "gof2/platform/gl.h"

// Cross-object reads of Engine/Mesh are deferred to a later Engine/Mesh-modeling pass; until
// then their fields are reached by raw offset.
template <class T> static inline T &ae_field(void *base, unsigned int off) {
    return *(T *)((char *)base + off);
}

// PostBWShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char PostBWShader_vtable;

// Engine entry points reached by opaque pointer (modelled in a later pass).
extern "C" unsigned int AbyssEngine_Engine_GetDisplayWidth(AbyssEngine::Engine *engine);
extern "C" unsigned int AbyssEngine_Engine_GetDisplayHeight(AbyssEngine::Engine *engine);
extern "C" void AbyssEngine_Engine_SetWorldViewMatrix(AbyssEngine::Engine *engine,
                                                      const uint32_t *matrix);
extern "C" void AbyssEngine_Engine_DrawQuad(AbyssEngine::Engine *engine, int x, int y, int width,
                                            int height);

namespace AbyssEngine {

PostBWShader::PostBWShader()
{
    this->vtable = &PostBWShader_vtable + 8;
    this->name.s = u"PostBWShader";
}

// Compiles and links the grayscale post-process program and binds its attribute/uniform slots.
void PostBWShader::Init(Engine *)
{
    this->program = ES2LoadProgram(
        "attribute vec4 a_Position;attribute vec2 a_TexCoord;varying vec2 v_TexCoord;uniform mat4 u_MVPMatrix;void main(){gl_Position=u_MVPMatrix*a_Position;v_TexCoord=a_TexCoord;}",
        "precision mediump float;varying vec2 v_TexCoord;uniform sampler2D s_Texture;void main(){vec4 c=texture2D(s_Texture,v_TexCoord);float g=(c.r+c.g+c.b)/3.0;gl_FragColor=vec4(g,g,g,c.a);}");
    this->aPosition = glGetAttribLocation(this->program, "a_Position");
    this->aTexCoord = glGetAttribLocation(this->program, "a_TexCoord");
    this->uMvpMatrix = glGetUniformLocation(this->program, "u_MVPMatrix");
    this->sTexture = glGetUniformLocation(this->program, "s_Texture");
    glUseProgram(this->program);
    glUniform1i(this->sTexture, 0);
}

void PostBWShader::SetInActive()
{
    glDisableVertexAttribArray(this->aPosition);
    glDisableVertexAttribArray(this->aTexCoord);
}

// Binds the world matrix and the mesh's vertex/texcoord arrays (VBO or client-side).
void PostBWShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(this->uMvpMatrix, 1, 0, (float *)((char *)engine + 0x104));
    this->dirty = 0;

    glEnableVertexAttribArray(this->aPosition);
    glEnableVertexAttribArray(this->aTexCoord);

    if (ae_field<uint8_t>(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, ae_field<uint32_t>(mesh, 0x60));
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, ae_field<uint32_t>(mesh, 0x68));
        glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, 0);
    } else {
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, ae_field<void *>(mesh, 0x4));
        glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, ae_field<void *>(mesh, 0x8));
    }
}

// Draws the full-screen grayscale pass over the captured scene framebuffer.
void PostBWShader::RenderEffect(FBOContainer *fbo, Engine *engine)
{
    typedef unsigned int u32x4 __attribute__((vector_size(16), aligned(4)));
    u32x4 zero = {0, 0, 0, 0};
    ae_field<u32x4>(engine, 0x3b4) = zero;
    ae_field<u32x4>(engine, 0x3a4) = zero;
    ae_field<u32x4>(engine, 0x394) = zero;
    ae_field<u32x4>(engine, 0x384) = zero;
    ae_field<uint32_t>(engine, 0x3e4) = this->program;

    ae_field<float>(engine, 0x384) = 2.0f / (float)AbyssEngine_Engine_GetDisplayWidth(engine);
    ae_field<float>(engine, 0x398) = -(2.0f / (float)AbyssEngine_Engine_GetDisplayHeight(engine));
    ae_field<uint32_t>(engine, 0x3ac) = 0xbf800000;
    ae_field<uint32_t>(engine, 0x3b4) = 0xbf800000;
    ae_field<uint32_t>(engine, 0x3b8) = 0x3f800000;
    ae_field<uint32_t>(engine, 0x3c0) = 0x3f800000;

    float matrix[16] = {};
    matrix[0] = 1.0f;
    matrix[5] = 1.0f;
    matrix[14] = 1.0f;
    AbyssEngine_Engine_SetWorldViewMatrix(engine, (const uint32_t *)matrix);

    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);
    glUseProgram(this->program);
    glActiveTexture(0x84c0);
    fbo->Activate();
    glBindFramebuffer(0x8d40, ae_field<uint32_t>(engine, 0x40c));
    glClear(0x4100);

    int width;
    int height;
    if (*(int *)((char *)*(void **)(ae_field<void *>(engine, 0x30)) + 0x30) == 2) {
        width = AbyssEngine_Engine_GetDisplayWidth(engine);
        height = AbyssEngine_Engine_GetDisplayHeight(engine);
    } else {
        width = AbyssEngine_Engine_GetDisplayHeight(engine);
        height = AbyssEngine_Engine_GetDisplayWidth(engine);
    }
    glViewport(0, 0, width, height);

    glEnableVertexAttribArray(this->aPosition);
    glEnableVertexAttribArray(this->aTexCoord);
    glUniformMatrix4fv(this->uMvpMatrix, 1, 0, (float *)((char *)engine + 0x104));
    glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 0x4));
    glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 0x8));

    glClear(0x4000);
    AbyssEngine_Engine_DrawQuad(engine, 0, 0, AbyssEngine_Engine_GetDisplayWidth(engine),
                                AbyssEngine_Engine_GetDisplayHeight(engine));

    glDisableVertexAttribArray(this->aPosition);
    glDisableVertexAttribArray(this->aTexCoord);
    glEnable(0xbe2);
    glBlendFunc(0x302, 0x303);
    glActiveTexture(0x84c0);
    ae_field<uint32_t>(engine, 0x7c) = 0xffffffff;
}

} // namespace AbyssEngine
