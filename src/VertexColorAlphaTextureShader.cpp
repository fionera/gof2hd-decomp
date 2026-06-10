#include "gof2/VertexColorAlphaTextureShader.h"

extern "C" void *_ZN11AbyssEngine16ShaderBaseStructC2Ev(AbyssEngine::ShaderBaseStruct *self);

// Engine / Mesh are opaque here; their fields are accessed by raw offset since
// their real layouts live in other (non-batch) classes.
static inline int &eng_i32(void *self, uint32_t offset) { return *(int *)((char *)self + offset); }
static inline uint8_t &eng_u8(void *self, uint32_t offset) { return *(uint8_t *)((char *)self + offset); }
static inline float &eng_f32(void *self, uint32_t offset) { return *(float *)((char *)self + offset); }
static inline void *eng_ptr(void *self, uint32_t offset) { return *(void **)((char *)self + offset); }

// ---- Init_91b6c.cpp ----
namespace AbyssEngine {

void VertexColorAlphaTextureShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("VCATShader.vsh", "VCATShader.fsh");
    this->field_0x4 = program;

    this->field_0x34 = glGetAttribLocation(program, "a0");
    this->field_0x20 = glGetAttribLocation(this->field_0x4, "a1");
    this->field_0x24 = glGetAttribLocation(this->field_0x4, "a2");
    this->field_0x28 = glGetAttribLocation(this->field_0x4, "a3");
    this->field_0x2c = glGetAttribLocation(this->field_0x4, "a4");
    this->field_0x30 = glGetAttribLocation(this->field_0x4, "a5");

    this->field_0x38 = glGetUniformLocation(this->field_0x4, "u0");
    this->field_0x3c = glGetUniformLocation(this->field_0x4, "u1");
    this->field_0x40 = glGetUniformLocation(this->field_0x4, "u2");
    this->field_0x44 = glGetUniformLocation(this->field_0x4, "u3");
    this->field_0x48 = glGetUniformLocation(this->field_0x4, "u4");
    this->field_0x4c = glGetUniformLocation(this->field_0x4, "u5");
    this->field_0x50 = glGetUniformLocation(this->field_0x4, "u6");
    this->field_0x54 = glGetUniformLocation(this->field_0x4, "u7");
    this->field_0x58 = glGetUniformLocation(this->field_0x4, "u8");

    glUseProgram(this->field_0x4);
    return glUniform1i(this->field_0x48, 0);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_91ce4.cpp ----
namespace AbyssEngine {

void VertexColorAlphaTextureShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->field_0x38 >= 0)
        glUniformMatrix4fv(this->field_0x38, 1, 0, (char *)engine + 0x104);
    if (this->field_0x3c >= 0)
        glUniformMatrix3fv(this->field_0x3c, 1, 0, (char *)engine + 0x204);

    if (this->field_0x9 != 0) {
        if (this->field_0x40 >= 0)
            glUniform3f(this->field_0x40, eng_f32(engine, 0x330), eng_f32(engine, 0x334),
                        eng_f32(engine, 0x338));
        if (this->field_0x44 >= 0)
            glUniform3f(this->field_0x44, eng_f32(engine, 0x34c), eng_f32(engine, 0x350),
                        eng_f32(engine, 0x354));
        if (this->field_0x4c >= 0)
            glUniform4fv(this->field_0x4c, 1, (float *)((char *)engine + 0xd0));
        if (this->field_0x50 >= 0)
            glUniform4fv(this->field_0x50, 1, (float *)((char *)engine + 0x2a8));
        if (this->field_0x54 >= 0)
            glUniform4fv(this->field_0x54, 1, (float *)((char *)engine + 0x298));
        if (this->field_0x58 >= 0)
            glUniform4fv(this->field_0x58, 1, (float *)((char *)engine + 0x2b8));
        this->field_0x9 = 0;
    }

    if (this->field_0x20 >= 0)
        glEnableVertexAttribArray(this->field_0x20);
    if (this->field_0x24 >= 0)
        glEnableVertexAttribArray(this->field_0x24);
    if (this->field_0x28 >= 0)
        glEnableVertexAttribArray(this->field_0x28);
    if (this->field_0x2c >= 0)
        glEnableVertexAttribArray(this->field_0x2c);
    if (this->field_0x30 >= 0)
        glEnableVertexAttribArray(this->field_0x30);
    if (this->field_0x34 >= 0)
        glEnableVertexAttribArray(this->field_0x34);

    if (eng_u8(mesh, 0x5c) == 0) {
        if (this->field_0x20 >= 0)
            glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, eng_ptr(mesh, 0x4));
        if (this->field_0x24 >= 0)
            glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, eng_ptr(mesh, 0x8));
        if (this->field_0x28 >= 0)
            glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, eng_ptr(mesh, 0x10));
        if (this->field_0x2c >= 0)
            glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, eng_ptr(mesh, 0x14));
        if (this->field_0x30 >= 0)
            glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, eng_ptr(mesh, 0x18));
        if (this->field_0x34 >= 0)
            glVertexAttribPointer(this->field_0x34, 4, 0x1406, 0, 0, eng_ptr(mesh, 0xc));
    } else {
        void (*bindBuffer)(uint32_t, uint32_t) = glBindBuffer;
        void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, eng_i32(mesh, 0x60));
        vertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, eng_i32(mesh, 0x68));
        vertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, eng_i32(mesh, 0x6c));
        vertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, eng_i32(mesh, 0x70));
        vertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, eng_i32(mesh, 0x74));
        vertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, eng_i32(mesh, 0x78));
        vertexAttribPointer(this->field_0x34, 4, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- SetInActive_91c9c.cpp ----
namespace AbyssEngine {

void VertexColorAlphaTextureShader::SetInActive()
{
    int loc;
    loc = this->field_0x20;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x24;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x28;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x2c;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x30;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x34;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

// ---- _VertexColorAlphaTextureShader_91f28.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine29VertexColorAlphaTextureShaderD0Ev(
    AbyssEngine::VertexColorAlphaTextureShader *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- VertexColorAlphaTextureShader_91ae0.cpp ----
namespace AbyssEngine {

// Global shader-index counter copied into a destination slot during construction.
__attribute__((visibility("hidden"))) extern int g_shaderIndexSrc;
__attribute__((visibility("hidden"))) extern int g_shaderIndexDst;

VertexColorAlphaTextureShader::VertexColorAlphaTextureShader()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);

    // Install the derived vtable (base address + 8 -> first virtual slot).
    this->field_0x0 = (void *)(_ZTVN11AbyssEngine29VertexColorAlphaTextureShaderE + 8);

    // Publish the current shader index.
    g_shaderIndexDst = g_shaderIndexSrc;

    // Store the shader resource name in the embedded String at +0x0c.
    String name;
    String_ctor_char(&name, "VCATShader", false);
    String_assign(&this->field_0xc, &name);
    String_dtor(&name);
}

} // namespace AbyssEngine
