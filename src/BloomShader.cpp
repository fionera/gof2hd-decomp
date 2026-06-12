#include "gof2/BloomShader.h"
#include "gof2/FBOContainer.h"
#include "gof2/Engine.h"

// cross-class field accessors (Engine/Mesh/FBOContainer are not in this batch; opaque here)
template <class T> static inline T &ae_field(void *base, unsigned int off) {
    return *(T *)((char *)base + off);
}

extern "C" int glGetAttribLocation(unsigned int program, const char *name);
extern "C" int glGetUniformLocation(unsigned int program, const char *name);
extern "C" void glUseProgram(unsigned int program);
extern "C" void glUniform1i(int location, int value);
void *_ZN11AbyssEngine11BloomShaderD1Ev(AbyssEngine::BloomShader *self);
extern "C" void operator_delete(void *self);
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
extern "C" unsigned char g_BloomShader_internalInitNeeded;
extern "C" unsigned int g_BloomShader_shaderMode;
extern "C" void glDisableVertexAttribArray_thunk(unsigned int index);
extern "C" void ShaderBaseStruct_ctor(void *self);
extern "C" char BloomShader_vtable;
extern "C" void *BloomShader_typeinfo_source;
extern "C" void *BloomShader_typeinfo_dest;
extern "C" void FBOContainer_ctor(void *self, void *engine, String *name);
extern "C" void *ShaderBaseStruct_dtor(void *self);

// ---- Init_895ec.cpp ----

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
    this->field_0x20 =
        ((ShaderBaseStruct *)this)->ES2LoadProgram(vertex, "BloomShaderDownSample.fsh");
    this->field_0x38 =
        ((ShaderBaseStruct *)this)->ES2LoadProgram(vertex, "BloomShaderBlurH.fsh");
    this->field_0x54 =
        ((ShaderBaseStruct *)this)->ES2LoadProgram(vertex, "BloomShaderBlurV.fsh");
    this->field_0x74 =
        ((ShaderBaseStruct *)this)->ES2LoadProgram(vertex, "BloomShaderFinal.fsh");

    this->field_0x8c = attrib(this->field_0x4, positionName);
    this->field_0x94 = attrib(this->field_0x4, texCoordName);
    this->field_0x90 = uniform(this->field_0x4, matrixName);
    this->field_0x98 = uniform(this->field_0x4, samplerName);
    useProgram(this->field_0x4);
    uniform1i(this->field_0x98, 0);

    this->field_0x24 = attrib(this->field_0x20, positionName);
    this->field_0x2c = attrib(this->field_0x20, texCoordName);
    this->field_0x28 = uniform(this->field_0x20, matrixName);
    this->field_0x30 = uniform(this->field_0x20, samplerName);
    useProgram(this->field_0x20);
    uniform1i(this->field_0x30, 0);

    this->field_0x3c = attrib(this->field_0x38, positionName);
    this->field_0x44 = attrib(this->field_0x38, texCoordName);
    this->field_0x40 = uniform(this->field_0x38, matrixName);
    this->field_0x48 = uniform(this->field_0x38, samplerName);
    this->field_0x4c = uniform(this->field_0x38, texSizeName);
    useProgram(this->field_0x38);
    uniform1i(this->field_0x48, 0);

    this->field_0x58 = attrib(this->field_0x54, positionName);
    this->field_0x60 = attrib(this->field_0x54, texCoordName);
    this->field_0x5c = uniform(this->field_0x54, matrixName);
    this->field_0x64 = uniform(this->field_0x54, samplerName);
    this->field_0x68 = uniform(this->field_0x54, texSizeName);
    useProgram(this->field_0x54);
    uniform1i(this->field_0x64, 0);

    this->field_0x78 = attrib(this->field_0x74, positionName);
    this->field_0x80 = attrib(this->field_0x74, texCoordName);
    this->field_0x7c = uniform(this->field_0x74, matrixName);
    this->field_0x84 = uniform(this->field_0x74, samplerName);
    this->field_0x88 =
        uniform(this->field_0x74, "s_texture_bloom");
    useProgram(this->field_0x74);
    uniform1i(this->field_0x84, 0);
    return uniform1i(this->field_0x88, 1);
}

} // namespace AbyssEngine

// ---- _BloomShader_8947c.cpp ----
void _ZN11AbyssEngine11BloomShaderD0Ev(AbyssEngine::BloomShader *self)
{
    return operator_delete(_ZN11AbyssEngine11BloomShaderD1Ev(self));
}

// ---- UpdateMeshData_8a158.cpp ----
extern "C" void glUniformMatrix4fv(int location, int count, unsigned char transpose,
                                   const void *value);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type,
                                      unsigned char normalized, int stride, const void *pointer);

namespace AbyssEngine {

void BloomShader::UpdateMeshData(Mesh *mesh, ::Engine *engine)
{
    glUniformMatrix4fv(this->field_0x90, 1, 0, (char *)engine + 0x104);
    if (this->field_0x9 != 0) {
        this->field_0x9 = 0;
    }

    glEnableVertexAttribArray(this->field_0x8c);
    glEnableVertexAttribArray(this->field_0x94);

    unsigned int zero = 0;
    if (ae_field<uint8_t>(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, ae_field<unsigned int>(mesh, 0x60));
        glVertexAttribPointer(this->field_0x8c, 3, 0x1406, 0, 0,
                              (void *)zero);
        glBindBuffer(0x8892, ae_field<unsigned int>(mesh, 0x68));
        return glVertexAttribPointer(this->field_0x94, 2, 0x1406, 0, 0,
                                     (void *)zero);
    } else {
        glVertexAttribPointer(this->field_0x8c, 3, 0x1406, 0, 0,
                              ae_field<void *>(mesh, 0x4));
        return glVertexAttribPointer(this->field_0x94, 2, 0x1406, 0, 0,
                                     ae_field<void *>(mesh, 0x8));
    }
}

} // namespace AbyssEngine

// ---- RenderEffect_89820.cpp ----
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
        ((FBOContainer *)(this->field_0x70))->BeginCapture();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(0x4000);
        ((FBOContainer *)(this->field_0x70))->EndCapture();
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

    glUseProgram(this->field_0x20);
    glActiveTexture(0x84c0);
    ((FBOContainer *)(source))->Activate();
    ((FBOContainer *)(this->field_0x34))->BeginCapture();
    glEnableVertexAttribArray(this->field_0x24);
    glEnableVertexAttribArray(this->field_0x2c);
    const void *mvp = (char *)engine + 0x104;
    glUniformMatrix4fv(this->field_0x28, 1, 0, mvp);
    glVertexAttribPointer(this->field_0x24, 3, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 4));
    glVertexAttribPointer(this->field_0x2c, 2, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 8));
    glClear(0x4000);
    ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
    glDisableVertexAttribArray(this->field_0x24);
    glDisableVertexAttribArray(this->field_0x2c);

    FBOContainer *blurSource = this->field_0x34;
    for (int i = 6; i != 0; i -= 1) {
        glUseProgram(this->field_0x38);
        glActiveTexture(0x84c0);
        ((FBOContainer *)(blurSource))->Activate();
        ((FBOContainer *)(this->field_0x50))->BeginCapture();
        glEnableVertexAttribArray(this->field_0x3c);
        glEnableVertexAttribArray(this->field_0x44);
        glUniformMatrix4fv(this->field_0x40, 1, 0, mvp);
        glVertexAttribPointer(this->field_0x3c, 3, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 4));
        glVertexAttribPointer(this->field_0x44, 2, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 8));
        glUniform1f(this->field_0x4c,
                    (float)*(int *)((char *)this->field_0x50 + 0xc));
        glClear(0x4000);
        ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
        glDisableVertexAttribArray(this->field_0x3c);
        glDisableVertexAttribArray(this->field_0x44);

        glUseProgram(this->field_0x54);
        glActiveTexture(0x84c0);
        ((FBOContainer *)(this->field_0x50))->Activate();
        ((FBOContainer *)(this->field_0x6c))->BeginCapture();
        glEnableVertexAttribArray(this->field_0x58);
        glEnableVertexAttribArray(this->field_0x60);
        glUniformMatrix4fv(this->field_0x5c, 1, 0, mvp);
        glVertexAttribPointer(this->field_0x58, 3, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 4));
        glVertexAttribPointer(this->field_0x60, 2, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 8));
        glUniform1f(this->field_0x68,
                    (float)*(int *)((char *)this->field_0x6c + 0x10));
        glClear(0x4000);
        ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
        glDisableVertexAttribArray(this->field_0x58);
        glDisableVertexAttribArray(this->field_0x60);
        blurSource = this->field_0x6c;
    }

    FBOContainer *base = source;
    FBOContainer *bloom = this->field_0x70;
    if (g_BloomShader_shaderMode < 4) {
        switch (g_BloomShader_shaderMode) {
        case 1:
            base = this->field_0x34;
            break;
        case 2:
            base = this->field_0x6c;
            break;
        case 3:
            bloom = this->field_0x70;
            break;
        default:
            break;
        }
    }

    glUseProgram(this->field_0x74);
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

    glEnableVertexAttribArray(this->field_0x78);
    glEnableVertexAttribArray(this->field_0x80);
    glUniformMatrix4fv(this->field_0x7c, 1, 0, mvp);
    glVertexAttribPointer(this->field_0x78, 3, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 4));
    glVertexAttribPointer(this->field_0x80, 2, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 8));
    glClear(0x4000);
    ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
    glDisableVertexAttribArray(this->field_0x78);
    glDisableVertexAttribArray(this->field_0x80);
    glEnable(0xbe2);
    glBlendFunc(0x302, 0x303);
    glActiveTexture(0x84c0);

    return;
}

} // namespace AbyssEngine

// ---- SetInActive_897f4.cpp ----
namespace AbyssEngine {

void BloomShader::SetInActive()
{
    glDisableVertexAttribArray(this->field_0x8c);
    glDisableVertexAttribArray(this->field_0x94);
    glDisableVertexAttribArray(this->field_0x2c);
    return glDisableVertexAttribArray(this->field_0x24);
}

} // namespace AbyssEngine

// ---- BloomShader_893dc.cpp ----
namespace AbyssEngine {

__attribute__((minsize)) BloomShader::BloomShader()
{
    ShaderBaseStruct_ctor((ShaderBaseStruct *)this);
    this->field_0x0 = &BloomShader_vtable + 8;
    *(void **)BloomShader_typeinfo_dest = *(void **)BloomShader_typeinfo_source;
    this->field_0xc.s = u"BloomShader";
    return;
}

} // namespace AbyssEngine

// ---- InternalInit_8948c.cpp ----


namespace AbyssEngine {

void BloomShader::InternalInit(::Engine *engine)
{

    FBOContainer *fbo = (FBOContainer *)operator new(0x38);
    String luma; luma.s = u"BloomShader fboLuma";
    FBOContainer_ctor(fbo, engine, &luma);
    this->field_0x34 = fbo;
    ((FBOContainer *)(this->field_0x34))->Create(0x100, 0x100, true, false);

    fbo = (FBOContainer *)operator new(0x38);
    String blurH; blurH.s = u"BloomShader fboBlurH";
    FBOContainer_ctor(fbo, engine, &blurH);
    this->field_0x50 = fbo;
    ((FBOContainer *)(this->field_0x50))->Create(0x100, 0x100, true, false);

    fbo = (FBOContainer *)operator new(0x38);
    String blurV; blurV.s = u"BloomShader fboBlurV";
    FBOContainer_ctor(fbo, engine, &blurV);
    this->field_0x6c = fbo;
    ((FBOContainer *)(this->field_0x6c))->Create(0x100, 0x100, true, false);

    fbo = (FBOContainer *)operator new(0x38);
    String black; black.s = u"BloomShader fboBlack";
    FBOContainer_ctor(fbo, engine, &black);
    this->field_0x70 = fbo;
    ((FBOContainer *)(this->field_0x70))->Create(0x100, 0x100, true, false);

    return;
}

} // namespace AbyssEngine

// ---- _BloomShader_89468.cpp ----
void *_ZN11AbyssEngine11BloomShaderD1Ev(AbyssEngine::BloomShader *self)
{
    *(void **)self = &BloomShader_vtable + 8;
    return ShaderBaseStruct_dtor((ShaderBaseStruct *)self);
}

// ---- RenderEffect_89cd8.cpp ----
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
        ((FBOContainer *)(this->field_0x70))->BeginCapture();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(0x4000);
        ((FBOContainer *)(this->field_0x70))->EndCapture();
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

    glUseProgram(this->field_0x20);
    glActiveTexture(0x84c0);
    ((FBOContainer *)(source))->Activate();
    ((FBOContainer *)(this->field_0x34))->BeginCapture();
    glEnableVertexAttribArray(this->field_0x24);
    glEnableVertexAttribArray(this->field_0x2c);
    const void *mvp = (char *)engine + 0x104;
    glUniformMatrix4fv(this->field_0x28, 1, 0, mvp);
    glVertexAttribPointer(this->field_0x24, 3, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 4));
    glVertexAttribPointer(this->field_0x2c, 2, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 8));
    glClear(0x4000);
    ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
    glDisableVertexAttribArray(this->field_0x24);
    glDisableVertexAttribArray(this->field_0x2c);

    FBOContainer *blurSource = this->field_0x34;
    for (int i = 6; i != 0; i -= 1) {
        glUseProgram(this->field_0x38);
        glActiveTexture(0x84c0);
        ((FBOContainer *)(blurSource))->Activate();
        ((FBOContainer *)(this->field_0x50))->BeginCapture();
        glEnableVertexAttribArray(this->field_0x3c);
        glEnableVertexAttribArray(this->field_0x44);
        glUniformMatrix4fv(this->field_0x40, 1, 0, mvp);
        glVertexAttribPointer(this->field_0x3c, 3, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 4));
        glVertexAttribPointer(this->field_0x44, 2, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 8));
        glUniform1f(this->field_0x4c,
                    (float)*(int *)((char *)this->field_0x50 + 0xc));
        glClear(0x4000);
        ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
        glDisableVertexAttribArray(this->field_0x3c);
        glDisableVertexAttribArray(this->field_0x44);

        glUseProgram(this->field_0x54);
        glActiveTexture(0x84c0);
        ((FBOContainer *)(this->field_0x50))->Activate();
        ((FBOContainer *)(this->field_0x6c))->BeginCapture();
        glEnableVertexAttribArray(this->field_0x58);
        glEnableVertexAttribArray(this->field_0x60);
        glUniformMatrix4fv(this->field_0x5c, 1, 0, mvp);
        glVertexAttribPointer(this->field_0x58, 3, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 4));
        glVertexAttribPointer(this->field_0x60, 2, 0x1406, 0, 0,
                              *(void **)(ae_field<char *>(engine, 0x380) + 8));
        glUniform1f(this->field_0x68,
                    (float)*(int *)((char *)this->field_0x6c + 0x10));
        glClear(0x4000);
        ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
        glDisableVertexAttribArray(this->field_0x58);
        glDisableVertexAttribArray(this->field_0x60);
        blurSource = this->field_0x6c;
    }

    FBOContainer *base = source;
    FBOContainer *bloom = this->field_0x70;
    if (g_BloomShader_shaderMode < 4) {
        switch (g_BloomShader_shaderMode) {
        case 1:
            base = this->field_0x34;
            break;
        case 2:
            base = this->field_0x6c;
            break;
        case 3:
            bloom = this->field_0x70;
            break;
        default:
            break;
        }
    }

    glUseProgram(this->field_0x74);
    glActiveTexture(0x84c0);
    ((FBOContainer *)(base))->Activate();
    glActiveTexture(0x84c1);
    ((FBOContainer *)(bloom))->Activate();
    if (*target != 0) {
        ((FBOContainer *)(*target))->BeginCapture();
    }

    glEnableVertexAttribArray(this->field_0x78);
    glEnableVertexAttribArray(this->field_0x80);
    glUniformMatrix4fv(this->field_0x7c, 1, 0, mvp);
    glVertexAttribPointer(this->field_0x78, 3, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 4));
    glVertexAttribPointer(this->field_0x80, 2, 0x1406, 0, 0,
                          *(void **)(ae_field<char *>(engine, 0x380) + 8));
    glClear(0x4000);
    ((::Engine *)(engine))->DrawQuad(0, 0, ((::Engine *)(engine))->GetDisplayWidth(), ((::Engine *)(engine))->GetDisplayHeight());
    glDisableVertexAttribArray(this->field_0x78);
    glDisableVertexAttribArray(this->field_0x80);
    glEnable(0xbe2);
    glBlendFunc(0x302, 0x303);
    glActiveTexture(0x84c0);
    if (*target != 0) {
        ((FBOContainer *)(*target))->EndCapture();
    }

    return;
}

} // namespace AbyssEngine
