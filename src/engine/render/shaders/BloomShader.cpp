#include "gof2/engine/render/shaders/BloomShader.h"
#include "gof2/externs.h"
#include "gof2/engine/render/FBOContainer.h"
#include "gof2/engine/render/Engine.h"

// cross-class field accessors (Engine/Mesh/FBOContainer are not in this batch; opaque here)
template <class T> static inline T &ae_field(void *base, unsigned int off) {
    return *(T *)((char *)base + off);
}

extern "C" int glGetAttribLocation(unsigned int program, const char *name);
extern "C" int glGetUniformLocation(unsigned int program, const char *name);
extern "C" void glUseProgram(unsigned int program);
extern "C" void glUniform1i(int location, int value);
void *_ZN11AbyssEngine11BloomShaderD1Ev(AbyssEngine::BloomShader *self);
extern "C" void glEnableVertexAttribArray(unsigned int index);
extern "C" void glBindBuffer(unsigned int target, unsigned int buffer);
extern "C" void glDisable(unsigned int cap);
extern "C" void glEnable(unsigned int cap);
extern "C" void glDepthMask(unsigned char flag);
extern "C" void glActiveTexture(unsigned int texture);
extern "C" void glBindFramebuffer(unsigned int target, unsigned int framebuffer);
extern "C" void glDisableVertexAttribArray(unsigned int index);
extern "C" void glUniform1f(int location, float value);
extern "C" void glClearColor(float red, float green, float blue, float alpha);
extern "C" void glClear(unsigned int mask);
extern "C" void glBlendFunc(unsigned int sfactor, unsigned int dfactor);
extern "C" void glDisableVertexAttribArray_thunk(unsigned int index);
extern "C" char BloomShader_vtable;
extern "C" void *BloomShader_typeinfo_source;
extern "C" void *BloomShader_typeinfo_dest;
extern "C" void FBOContainer_ctor(void *self, void *engine, String *name);

namespace AbyssEngine {

void BloomShader::Init(::Engine *)
{
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

    this->field_0x4 =
        ((ShaderBaseStruct *)this)->ES2LoadProgram(vertex, "BloomShaderLuma.fsh");
    this->downSampleProgram =
        ((ShaderBaseStruct *)this)->ES2LoadProgram(vertex, "BloomShaderDownSample.fsh");
    this->blurHProgram =
        ((ShaderBaseStruct *)this)->ES2LoadProgram(vertex, "BloomShaderBlurH.fsh");
    this->blurVProgram =
        ((ShaderBaseStruct *)this)->ES2LoadProgram(vertex, "BloomShaderBlurV.fsh");
    this->finalProgram =
        ((ShaderBaseStruct *)this)->ES2LoadProgram(vertex, "BloomShaderFinal.fsh");

    this->lumaAttribPosition = attrib(this->field_0x4, positionName);
    this->lumaAttribTexCoord = attrib(this->field_0x4, texCoordName);
    this->lumaUniformWorldMatrix = uniform(this->field_0x4, matrixName);
    this->lumaUniformTexture = uniform(this->field_0x4, samplerName);
    useProgram(this->field_0x4);
    uniform1i(this->lumaUniformTexture, 0);

    this->downSampleAttribPosition = attrib(this->downSampleProgram, positionName);
    this->downSampleAttribTexCoord = attrib(this->downSampleProgram, texCoordName);
    this->downSampleUniformWorldMatrix = uniform(this->downSampleProgram, matrixName);
    this->downSampleUniformTexture = uniform(this->downSampleProgram, samplerName);
    useProgram(this->downSampleProgram);
    uniform1i(this->downSampleUniformTexture, 0);

    this->blurHAttribPosition = attrib(this->blurHProgram, positionName);
    this->blurHAttribTexCoord = attrib(this->blurHProgram, texCoordName);
    this->blurHUniformWorldMatrix = uniform(this->blurHProgram, matrixName);
    this->blurHUniformTexture = uniform(this->blurHProgram, samplerName);
    this->blurHUniformTexSize = uniform(this->blurHProgram, texSizeName);
    useProgram(this->blurHProgram);
    uniform1i(this->blurHUniformTexture, 0);

    this->blurVAttribPosition = attrib(this->blurVProgram, positionName);
    this->blurVAttribTexCoord = attrib(this->blurVProgram, texCoordName);
    this->blurVUniformWorldMatrix = uniform(this->blurVProgram, matrixName);
    this->blurVUniformTexture = uniform(this->blurVProgram, samplerName);
    this->blurVUniformTexSize = uniform(this->blurVProgram, texSizeName);
    useProgram(this->blurVProgram);
    uniform1i(this->blurVUniformTexture, 0);

    this->finalAttribPosition = attrib(this->finalProgram, positionName);
    this->finalAttribTexCoord = attrib(this->finalProgram, texCoordName);
    this->finalUniformWorldMatrix = uniform(this->finalProgram, matrixName);
    this->finalUniformTexture = uniform(this->finalProgram, samplerName);
    this->finalUniformTextureBloom =
        uniform(this->finalProgram, "s_texture_bloom");
    useProgram(this->finalProgram);
    uniform1i(this->finalUniformTexture, 0);
    return uniform1i(this->finalUniformTextureBloom, 1);
}

} // namespace AbyssEngine

void _ZN11AbyssEngine11BloomShaderD0Ev(AbyssEngine::BloomShader *self)
{
    return ::operator delete(_ZN11AbyssEngine11BloomShaderD1Ev(self));
}

extern "C" void glUniformMatrix4fv(int location, int count, unsigned char transpose,
                                   const void *value);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type,
                                      unsigned char normalized, int stride, const void *pointer);

namespace AbyssEngine {

void BloomShader::UpdateMeshData(Mesh *mesh, ::Engine *engine)
{
    glUniformMatrix4fv(this->lumaUniformWorldMatrix, 1, 0, (char *)engine + 0x104);
    if (this->lightingDirty != 0) {
        this->lightingDirty = 0;
    }

    glEnableVertexAttribArray(this->lumaAttribPosition);
    glEnableVertexAttribArray(this->lumaAttribTexCoord);

    unsigned int zero = 0;
    if (ae_field<uint8_t>(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, ae_field<unsigned int>(mesh, 0x60));
        glVertexAttribPointer(this->lumaAttribPosition, 3, 0x1406, 0, 0,
                              (void *)zero);
        glBindBuffer(0x8892, ae_field<unsigned int>(mesh, 0x68));
        return glVertexAttribPointer(this->lumaAttribTexCoord, 2, 0x1406, 0, 0,
                                     (void *)zero);
    } else {
        glVertexAttribPointer(this->lumaAttribPosition, 3, 0x1406, 0, 0,
                              ae_field<void *>(mesh, 0x4));
        return glVertexAttribPointer(this->lumaAttribTexCoord, 2, 0x1406, 0, 0,
                                     ae_field<void *>(mesh, 0x8));
    }
}

} // namespace AbyssEngine

extern "C" void glUniformMatrix4fv(int location, int count, unsigned char transpose,
                                   const void *value);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type,
                                      unsigned char normalized, int stride, const void *pointer);

namespace AbyssEngine {

void BloomShader::RenderEffect(FBOContainer *source, ::Engine *engine)
{
    ae_field<int>(engine, 0x3e4) = this->field_0x4;

    if (g_BloomShader_internalInitNeeded != 0) {
        g_BloomShader_internalInitNeeded = 0;
        InternalInit(engine);
        ((FBOContainer *)(this->fboBlack))->BeginCapture();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(0x4000);
        ((FBOContainer *)(this->fboBlack))->EndCapture();
    }

    typedef unsigned int u32x4 __attribute__((vector_size(16), aligned(4)));
    u32x4 zero = {0, 0, 0, 0};
    ae_field<u32x4>(engine, 0x3b4) = zero;
    ae_field<u32x4>(engine, 0x3a4) = zero;
    ae_field<u32x4>(engine, 0x394) = zero;
    ae_field<u32x4>(engine, 0x384) = zero;
    float matrix[16] = {};
    matrix[0] = 1.0f;
    matrix[5] = 1.0f;
    matrix[10] = 1.0f;
    matrix[15] = 1.0f;
    ae_field<float>(engine, 0x384) = 2.0f / (float)((::Engine *)(engine))->GetDisplayWidth();
    ae_field<float>(engine, 0x398) = -(2.0f / (float)((::Engine *)(engine))->GetDisplayHeight());
    ae_field<unsigned int>(engine, 0x3ac) = 0xbf800000;
    ae_field<unsigned int>(engine, 0x3b4) = 0xbf800000;
    ae_field<unsigned int>(engine, 0x3b8) = 0x3f800000;
    ae_field<unsigned int>(engine, 0x3c0) = 0x3f800000;
    ((::Engine *)(engine))->SetWorldViewMatrix((const uint32_t *)matrix);

    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);

    glUseProgram(this->downSampleProgram);
    glActiveTexture(0x84c0);
    ((FBOContainer *)(source))->Activate();
    ((FBOContainer *)(this->fboLuma))->BeginCapture();
    glEnableVertexAttribArray(this->downSampleAttribPosition);
    glEnableVertexAttribArray(this->downSampleAttribTexCoord);
    const void *mvp = (char *)engine + 0x104;
    glUniformMatrix4fv(this->downSampleUniformWorldMatrix, 1, 0, mvp);
    glVertexAttribPointer(this->downSampleAttribPosition, 3, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 4));
    glVertexAttribPointer(this->downSampleAttribTexCoord, 2, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 8));
    glClear(0x4000);
    ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
    glDisableVertexAttribArray(this->downSampleAttribPosition);
    glDisableVertexAttribArray(this->downSampleAttribTexCoord);

    FBOContainer *blurSource = this->fboLuma;
    for (int i = 6; i != 0; i -= 1) {
        glUseProgram(this->blurHProgram);
        glActiveTexture(0x84c0);
        ((FBOContainer *)(blurSource))->Activate();
        ((FBOContainer *)(this->fboBlurH))->BeginCapture();
        glEnableVertexAttribArray(this->blurHAttribPosition);
        glEnableVertexAttribArray(this->blurHAttribTexCoord);
        glUniformMatrix4fv(this->blurHUniformWorldMatrix, 1, 0, mvp);
        glVertexAttribPointer(this->blurHAttribPosition, 3, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 4));
        glVertexAttribPointer(this->blurHAttribTexCoord, 2, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 8));
        glUniform1f(this->blurHUniformTexSize,
                    (float)*(int *)((char *)this->fboBlurH + 0xc));
        glClear(0x4000);
        ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
        glDisableVertexAttribArray(this->blurHAttribPosition);
        glDisableVertexAttribArray(this->blurHAttribTexCoord);

        glUseProgram(this->blurVProgram);
        glActiveTexture(0x84c0);
        ((FBOContainer *)(this->fboBlurH))->Activate();
        ((FBOContainer *)(this->fboBlurV))->BeginCapture();
        glEnableVertexAttribArray(this->blurVAttribPosition);
        glEnableVertexAttribArray(this->blurVAttribTexCoord);
        glUniformMatrix4fv(this->blurVUniformWorldMatrix, 1, 0, mvp);
        glVertexAttribPointer(this->blurVAttribPosition, 3, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 4));
        glVertexAttribPointer(this->blurVAttribTexCoord, 2, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 8));
        glUniform1f(this->blurVUniformTexSize,
                    (float)*(int *)((char *)this->fboBlurV + 0x10));
        glClear(0x4000);
        ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
        glDisableVertexAttribArray(this->blurVAttribPosition);
        glDisableVertexAttribArray(this->blurVAttribTexCoord);
        blurSource = this->fboBlurV;
    }

    FBOContainer *base = source;
    FBOContainer *bloom = this->fboBlack;
    if (g_BloomShader_shaderMode < 4) {
        switch (g_BloomShader_shaderMode) {
        case 1:
            base = this->fboLuma;
            break;
        case 2:
            base = this->fboBlurV;
            break;
        case 3:
            bloom = this->fboBlack;
            break;
        default:
            break;
        }
    }

    glUseProgram(this->finalProgram);
    glActiveTexture(0x84c0);
    ((FBOContainer *)(base))->Activate();
    glActiveTexture(0x84c1);
    ((FBOContainer *)(bloom))->Activate();
    glBindFramebuffer(0x8d40, ae_field<unsigned int>(engine, 0x40c));
    unsigned int width;
    unsigned int height;
    if (*(int *)(ae_field<char *>(engine, 0x30) + 0x30) == 2) {
        width = ((::Engine *)(engine))->GetDisplayWidth();
        height = ((::Engine *)(engine))->GetDisplayHeight();
    } else {
        width = ((::Engine *)(engine))->GetDisplayHeight();
        height = ((::Engine *)(engine))->GetDisplayWidth();
    }
    glViewport(0, 0, width, height);

    glEnableVertexAttribArray(this->finalAttribPosition);
    glEnableVertexAttribArray(this->finalAttribTexCoord);
    glUniformMatrix4fv(this->finalUniformWorldMatrix, 1, 0, mvp);
    glVertexAttribPointer(this->finalAttribPosition, 3, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 4));
    glVertexAttribPointer(this->finalAttribTexCoord, 2, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 8));
    glClear(0x4000);
    ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
    glDisableVertexAttribArray(this->finalAttribPosition);
    glDisableVertexAttribArray(this->finalAttribTexCoord);
    glEnable(0xbe2);
    glBlendFunc(0x302, 0x303);
    glActiveTexture(0x84c0);

    return;
}

} // namespace AbyssEngine

namespace AbyssEngine {

void BloomShader::SetInActive()
{
    glDisableVertexAttribArray(this->lumaAttribPosition);
    glDisableVertexAttribArray(this->lumaAttribTexCoord);
    glDisableVertexAttribArray(this->downSampleAttribTexCoord);
    return glDisableVertexAttribArray(this->downSampleAttribPosition);
}

} // namespace AbyssEngine

namespace AbyssEngine {

__attribute__((minsize)) BloomShader::BloomShader()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    this->field_0x0 = &BloomShader_vtable + 8;
    *(void **)BloomShader_typeinfo_dest = *(void **)BloomShader_typeinfo_source;
    this->name.s = u"BloomShader";
    return;
}

} // namespace AbyssEngine

namespace AbyssEngine {

void BloomShader::InternalInit(::Engine *engine)
{

    FBOContainer *fbo = (FBOContainer *)operator new(0x38);
    String luma; luma.s = u"BloomShader fboLuma";
    FBOContainer_ctor(fbo, engine, &luma);
    this->fboLuma = fbo;
    ((FBOContainer *)(this->fboLuma))->Create(0x100, 0x100, true, false);

    fbo = (FBOContainer *)operator new(0x38);
    String blurH; blurH.s = u"BloomShader fboBlurH";
    FBOContainer_ctor(fbo, engine, &blurH);
    this->fboBlurH = fbo;
    ((FBOContainer *)(this->fboBlurH))->Create(0x100, 0x100, true, false);

    fbo = (FBOContainer *)operator new(0x38);
    String blurV; blurV.s = u"BloomShader fboBlurV";
    FBOContainer_ctor(fbo, engine, &blurV);
    this->fboBlurV = fbo;
    ((FBOContainer *)(this->fboBlurV))->Create(0x100, 0x100, true, false);

    fbo = (FBOContainer *)operator new(0x38);
    String black; black.s = u"BloomShader fboBlack";
    FBOContainer_ctor(fbo, engine, &black);
    this->fboBlack = fbo;
    ((FBOContainer *)(this->fboBlack))->Create(0x100, 0x100, true, false);

    return;
}

} // namespace AbyssEngine

void *_ZN11AbyssEngine11BloomShaderD1Ev(AbyssEngine::BloomShader *self)
{
    *(void **)self = &BloomShader_vtable + 8;
    ((AbyssEngine::ShaderBaseStruct *)self)->~ShaderBaseStruct();
    return self;
}

extern "C" void glUniformMatrix4fv(int location, int count, unsigned char transpose,
                                   const void *value);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type,
                                      unsigned char normalized, int stride, const void *pointer);

namespace AbyssEngine {

void BloomShader::RenderEffect(FBOContainer *source, FBOContainer **target, ::Engine *engine)
{

    if (g_BloomShader_internalInitNeeded != 0) {
        g_BloomShader_internalInitNeeded = 0;
        InternalInit(engine);
        ((FBOContainer *)(this->fboBlack))->BeginCapture();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(0x4000);
        ((FBOContainer *)(this->fboBlack))->EndCapture();
    }

    typedef unsigned int u32x4 __attribute__((vector_size(16), aligned(4)));
    u32x4 zero = {0, 0, 0, 0};
    ae_field<u32x4>(engine, 0x3b4) = zero;
    ae_field<u32x4>(engine, 0x3a4) = zero;
    ae_field<u32x4>(engine, 0x394) = zero;
    ae_field<u32x4>(engine, 0x384) = zero;
    float matrix[16] = {};
    matrix[0] = 1.0f;
    matrix[5] = 1.0f;
    matrix[10] = 1.0f;
    matrix[15] = 1.0f;
    ae_field<float>(engine, 0x384) = 2.0f / (float)((::Engine *)(engine))->GetDisplayWidth();
    ae_field<float>(engine, 0x398) = -(2.0f / (float)((::Engine *)(engine))->GetDisplayHeight());
    ae_field<unsigned int>(engine, 0x3ac) = 0xbf800000;
    ae_field<unsigned int>(engine, 0x3b4) = 0xbf800000;
    ae_field<unsigned int>(engine, 0x3b8) = 0x3f800000;
    ae_field<unsigned int>(engine, 0x3c0) = 0x3f800000;
    ((::Engine *)(engine))->SetWorldViewMatrix((const uint32_t *)matrix);

    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);

    glUseProgram(this->downSampleProgram);
    glActiveTexture(0x84c0);
    ((FBOContainer *)(source))->Activate();
    ((FBOContainer *)(this->fboLuma))->BeginCapture();
    glEnableVertexAttribArray(this->downSampleAttribPosition);
    glEnableVertexAttribArray(this->downSampleAttribTexCoord);
    const void *mvp = (char *)engine + 0x104;
    glUniformMatrix4fv(this->downSampleUniformWorldMatrix, 1, 0, mvp);
    glVertexAttribPointer(this->downSampleAttribPosition, 3, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 4));
    glVertexAttribPointer(this->downSampleAttribTexCoord, 2, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 8));
    glClear(0x4000);
    ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
    glDisableVertexAttribArray(this->downSampleAttribPosition);
    glDisableVertexAttribArray(this->downSampleAttribTexCoord);

    FBOContainer *blurSource = this->fboLuma;
    for (int i = 6; i != 0; i -= 1) {
        glUseProgram(this->blurHProgram);
        glActiveTexture(0x84c0);
        ((FBOContainer *)(blurSource))->Activate();
        ((FBOContainer *)(this->fboBlurH))->BeginCapture();
        glEnableVertexAttribArray(this->blurHAttribPosition);
        glEnableVertexAttribArray(this->blurHAttribTexCoord);
        glUniformMatrix4fv(this->blurHUniformWorldMatrix, 1, 0, mvp);
        glVertexAttribPointer(this->blurHAttribPosition, 3, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 4));
        glVertexAttribPointer(this->blurHAttribTexCoord, 2, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 8));
        glUniform1f(this->blurHUniformTexSize,
                    (float)*(int *)((char *)this->fboBlurH + 0xc));
        glClear(0x4000);
        ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
        glDisableVertexAttribArray(this->blurHAttribPosition);
        glDisableVertexAttribArray(this->blurHAttribTexCoord);

        glUseProgram(this->blurVProgram);
        glActiveTexture(0x84c0);
        ((FBOContainer *)(this->fboBlurH))->Activate();
        ((FBOContainer *)(this->fboBlurV))->BeginCapture();
        glEnableVertexAttribArray(this->blurVAttribPosition);
        glEnableVertexAttribArray(this->blurVAttribTexCoord);
        glUniformMatrix4fv(this->blurVUniformWorldMatrix, 1, 0, mvp);
        glVertexAttribPointer(this->blurVAttribPosition, 3, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 4));
        glVertexAttribPointer(this->blurVAttribTexCoord, 2, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 8));
        glUniform1f(this->blurVUniformTexSize,
                    (float)*(int *)((char *)this->fboBlurV + 0x10));
        glClear(0x4000);
        ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
        glDisableVertexAttribArray(this->blurVAttribPosition);
        glDisableVertexAttribArray(this->blurVAttribTexCoord);
        blurSource = this->fboBlurV;
    }

    FBOContainer *base = source;
    FBOContainer *bloom = this->fboBlack;
    if (g_BloomShader_shaderMode < 4) {
        switch (g_BloomShader_shaderMode) {
        case 1:
            base = this->fboLuma;
            break;
        case 2:
            base = this->fboBlurV;
            break;
        case 3:
            bloom = this->fboBlack;
            break;
        default:
            break;
        }
    }

    glUseProgram(this->finalProgram);
    glActiveTexture(0x84c0);
    ((FBOContainer *)(base))->Activate();
    glActiveTexture(0x84c1);
    ((FBOContainer *)(bloom))->Activate();
    if (*target != 0) {
        ((FBOContainer *)(*target))->BeginCapture();
    }

    glEnableVertexAttribArray(this->finalAttribPosition);
    glEnableVertexAttribArray(this->finalAttribTexCoord);
    glUniformMatrix4fv(this->finalUniformWorldMatrix, 1, 0, mvp);
    glVertexAttribPointer(this->finalAttribPosition, 3, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 4));
    glVertexAttribPointer(this->finalAttribTexCoord, 2, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 8));
    glClear(0x4000);
    ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
    glDisableVertexAttribArray(this->finalAttribPosition);
    glDisableVertexAttribArray(this->finalAttribTexCoord);
    glEnable(0xbe2);
    glBlendFunc(0x302, 0x303);
    glActiveTexture(0x84c0);
    if (*target != 0) {
        ((FBOContainer *)(*target))->EndCapture();
    }

    return;
}

} // namespace AbyssEngine
