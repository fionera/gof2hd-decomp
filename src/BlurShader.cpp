#include "gof2/BlurShader.h"
#include "gof2/Engine.h"
#include "gof2/String.h"
// gof2/Mesh.h is pulled in by gof2/BlurShader.h. gof2/Engine.h is intentionally NOT included
// (it does not compile standalone); BlurShader.h provides a minimal Engine view instead.


extern "C" void glDisableVertexAttribArray(unsigned int index);
extern "C" void glDisableVertexAttribArray_thunk(unsigned int index);
extern "C" void ShaderBaseStruct_ctor(ShaderBaseStruct *self);
extern "C" void String_ctor_text(void *self, const char *text, bool copy);
extern "C" char BlurShader_vtable[];
extern "C" void *BlurShader_typeinfo_source[];
extern "C" void *BlurShader_typeinfo_dest[];
extern "C" int glGetAttribLocation(unsigned int program, const char *name);
extern "C" int glGetUniformLocation(unsigned int program, const char *name);
extern "C" void glUseProgram(unsigned int program);
extern "C" void glUniform1i(int location, int value);
extern "C" void glDisable(unsigned int cap);
extern "C" void glEnable(unsigned int cap);
extern "C" void glDepthMask(unsigned char flag);
extern "C" void glActiveTexture(unsigned int texture);
extern "C" void glBindFramebuffer(unsigned int target, unsigned int framebuffer);
extern "C" void glViewport(int x, int y, int width, int height);
extern "C" void glEnableVertexAttribArray(unsigned int index);
extern "C" void glUniform2f(int location, float x, float y);
extern "C" void glUniform1f(int location, float value);
extern "C" void glClear(unsigned int mask);
extern "C" void glBlendFunc(unsigned int sfactor, unsigned int dfactor);
extern "C" void FBOContainer_Activate(FBOContainer *self);
extern "C" void FBOContainer_BeginCapture(FBOContainer *self);
extern "C" void FBOContainer_EndCapture(FBOContainer *self);
extern "C" void *ShaderBaseStruct_dtor(ShaderBaseStruct *self);
extern "C" void operator_delete(void *self);
extern "C" void glBindBuffer(unsigned int target, unsigned int buffer);

// ---- SetInActive_8a748.cpp ----
namespace AbyssEngine {

void BlurShader::SetInActive()
{
    glDisableVertexAttribArray(this->field_0x48);
    return glDisableVertexAttribArray_thunk(this->field_0x50);
}

} // namespace AbyssEngine

// ---- BlurShader_8a5e0.cpp ----
extern "C" AbyssEngine::BlurShader *BlurShader_BlurShader(AbyssEngine::BlurShader *self)
{
    using AbyssEngine::BlurShader;
    void *volatile cookie = __stack_chk_guard;
    String name;
    ShaderBaseStruct_ctor((ShaderBaseStruct *)self);
    void **source = BlurShader_typeinfo_source;
    void **dest = BlurShader_typeinfo_dest;
    *(void **)self = BlurShader_vtable + 8;
    *dest = *source;
    ((String *)(&name))->ctor_char("BlurShader", false);
    ((String *)((char *)self + 0xc))->assign(&name);
    ((String *)(&name))->dtor();
    self->field_0x58 = 0x92006800;
    self->field_0x5c = 0x40000000;
    uint32_t guardDelta =
        stack_guard_delta(*(volatile uint32_t *)&cookie, (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard);
    if (guardDelta == 0) {
        return self;
    }
    __stack_chk_fail(guardDelta);
}

// ---- RenderEffect_8a760.cpp ----
typedef void FBOEffect(FBOContainer *, Engine *, void *, uint32_t, uint32_t, uint32_t, uint32_t,
                       uint32_t);

extern "C" void BlurShader_RenderEffect(FBOContainer *fbo, Engine *engine, uint32_t amount,
                                        uint32_t x, uint32_t y, uint32_t z, uint32_t w)
{
    void *volatile cookie = __stack_chk_guard;
    uint32_t zero = 0;
    void **vtable = *(void ***)fbo;
    ((FBOEffect *)vtable[0x20 / 4])(fbo, engine, &zero, amount, x, y, z, w);
    uint32_t guardDelta =
        stack_guard_delta(*(volatile uint32_t *)&cookie, (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard);
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

// ---- Init_8a68c.cpp ----
extern "C" unsigned int ShaderBaseStruct_ES2LoadProgram(ShaderBaseStruct *self,
                                                        const char *vertex,
                                                        const char *fragment);

namespace AbyssEngine {

void BlurShader::Init(::Engine *)
{
    unsigned int program = ShaderBaseStruct_ES2LoadProgram((::ShaderBaseStruct *)this,
                                                           "BlurShader.vsh",
                                                           "BlurShader.fsh");
    this->field_0x24 = program;
    this->field_0x28 = glGetAttribLocation(program, "a_position");
    this->field_0x30 =
        glGetAttribLocation(this->field_0x24, "a_texCoord");

    int (*uniform)(unsigned int, const char *) = glGetUniformLocation;
    this->field_0x2c =
        uniform(this->field_0x24, "u_mvpMatrix");
    this->field_0x34 =
        uniform(this->field_0x24, "s_texture");
    this->field_0x38 =
        uniform(this->field_0x24, "u_texelSize");
    this->field_0x44 =
        uniform(this->field_0x24, "u_center");
    this->field_0x3c =
        uniform(this->field_0x24, "u_blurAmount");
    this->field_0x40 =
        uniform(this->field_0x24, "u_strength");
    glUseProgram(this->field_0x24);
    return glUniform1i(this->field_0x34, 0);
}

} // namespace AbyssEngine

// ---- RenderEffect_8a7a8.cpp ----
extern "C" void glUniformMatrix4fv(int location, int count, unsigned char transpose,
                                   const void *value);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type,
                                      unsigned char normalized, int stride, const void *pointer);

namespace AbyssEngine {

void BlurShader::RenderEffect(::FBOContainer *fbo, ::FBOContainer **target, ::Engine *engine,
                              float amount, Vector vector)
{
    void *volatile cookie = __stack_chk_guard;
    float matrix[16] = {};
    matrix[0] = 1.0f;
    matrix[5] = 1.0f;
    matrix[10] = 1.0f;
    matrix[15] = 1.0f;

    engine->field_0x384 = 2.0f / (float)(engine)->GetDisplayWidth();
    engine->field_0x398 = -(2.0f / (float)(engine)->GetDisplayHeight());
    engine->field_0x3ac = 0xbf800000;
    engine->field_0x3b4 = 0xbf800000;
    engine->field_0x3b8 = 0x3f800000;
    engine->field_0x3c0 = 0x3f800000;
    (engine)->SetWorldViewMatrix((const uint32_t *)matrix);

    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);
    glUseProgram(this->field_0x24);
    glActiveTexture(0x84c0);
    FBOContainer_Activate(fbo);

    if (*target == 0) {
        glBindFramebuffer(0x8d40, engine->field_0x40c);
        int width;
        int height;
        if (*(int *)(*engine->field_0x30 + 0x30) == 2) {
            width = (engine)->GetDisplayWidth();
            height = (engine)->GetDisplayHeight();
        } else {
            width = (engine)->GetDisplayHeight();
            height = (engine)->GetDisplayWidth();
        }
        glViewport(0, 0, width, height);
    } else {
        FBOContainer_BeginCapture(*target);
    }

    int position = this->field_0x28;
    if (position >= 0) {
        glEnableVertexAttribArray(position);
    }
    int texCoord = this->field_0x30;
    if (texCoord >= 0) {
        glEnableVertexAttribArray(texCoord);
    }
    int matrixLocation = this->field_0x2c;
    if (matrixLocation >= 0) {
        glUniformMatrix4fv(matrixLocation, 1, 0, (char *)engine + 0x104);
    }
    int texelLocation = this->field_0x38;
    if (texelLocation >= 0) {
        float width;
        int other;
        if (*(int *)(*engine->field_0x30 + 0x30) == 2) {
            width = (float)(engine)->GetDisplayWidth();
            other = (engine)->GetDisplayHeight();
        } else {
            width = (float)(engine)->GetDisplayHeight();
            other = (engine)->GetDisplayWidth();
        }
        glUniform2f(texelLocation, 1.0f / width, 1.0f / (float)other);
    }
    int centerLocation = this->field_0x44;
    if (centerLocation >= 0) {
        float x = vector[0];
        float y = vector[1];
        glUniform2f(centerLocation, x, y);
    }
    int strengthLocation = this->field_0x40;
    if (strengthLocation >= 0) {
        glUniform1f(strengthLocation, this->field_0x58);
    }
    int amountLocation = this->field_0x3c;
    if (amountLocation >= 0) {
        float scaled = amount;
        if (scaled < 0.2f) {
            scaled = 0.2f;
        }
        glUniform1f(amountLocation, scaled * this->field_0x5c);
    }
    if (position >= 0) {
        glVertexAttribPointer(position, 3, 0x1406, 0, 0,
                              *(void **)(engine->field_0x380 + 4));
    }
    if (texCoord >= 0) {
        glVertexAttribPointer(texCoord, 2, 0x1406, 0, 0,
                              *(void **)(engine->field_0x380 + 8));
    }

    glClear(0x4000);
    int width = (engine)->GetDisplayWidth();
    (engine)->DrawQuad(0, 0, width, (engine)->GetDisplayHeight());
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

// ---- _BlurShader_8a67c.cpp ----
namespace AbyssEngine {

BlurShader::~BlurShader()
{
    operator_delete(ShaderBaseStruct_dtor((ShaderBaseStruct *)this));
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8aab8.cpp ----
extern "C" void glUniformMatrix4fv(int location, int count, unsigned char transpose,
                                   const void *value);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type,
                                      unsigned char normalized, int stride, const void *pointer);

namespace AbyssEngine {

void BlurShader::UpdateMeshData(AbyssEngine::Mesh *mesh, ::Engine *engine)
{
    int matrixLocation = this->field_0x2c;
    if (matrixLocation >= 0) {
        glUniformMatrix4fv(matrixLocation, 1, 0, (char *)engine + 0x104);
    }

    int texelLocation = this->field_0x38;
    if (texelLocation >= 0) {
        float width;
        float height;
        float one = 1.0f;
        if (*(int *)(*engine->field_0x30 + 0x30) == 2) {
            width = (float)(engine)->GetDisplayWidth();
            height = (float)(engine)->GetDisplayHeight();
        } else {
            width = (float)(engine)->GetDisplayHeight();
            height = (float)(engine)->GetDisplayWidth();
        }
        glUniform2f(texelLocation, one / width, one / height);
    }

    int centerLocation = this->field_0x44;
    if (centerLocation >= 0) {
        glUniform2f(centerLocation, 0.5f, 0.5f);
    }

    int strengthLocation = this->field_0x40;
    if (strengthLocation >= 0) {
        glUniform1f(strengthLocation, this->field_0x58);
    }

    int amountLocation = this->field_0x3c;
    if (amountLocation >= 0) {
        glUniform1f(amountLocation, this->field_0x5c);
    }

    if (this->field_0x9 != 0) {
        this->field_0x9 = 0;
    }

    glEnableVertexAttribArray(this->field_0x48);
    glEnableVertexAttribArray(this->field_0x50);

    const void *texPtr;
    if (mesh->field_0x5c == 0) {
        glVertexAttribPointer(this->field_0x48, 3, 0x1406, 0, 0,
                              mesh->field_0x4);
        unsigned int texCoord = this->field_0x50;
        texPtr = mesh->field_0x8;
        return glVertexAttribPointer(texCoord, 2, 0x1406, 0, 0, texPtr);
    } else {
        glBindBuffer(0x8892, mesh->field_0x60);
        glVertexAttribPointer(this->field_0x48, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x68);
        unsigned int texCoord = this->field_0x50;
        texPtr = 0;
        return glVertexAttribPointer(texCoord, 2, 0x1406, 0, 0, texPtr);
    }
}

} // namespace AbyssEngine
