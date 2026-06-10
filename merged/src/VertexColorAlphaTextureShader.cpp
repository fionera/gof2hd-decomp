#include "VertexColorAlphaTextureShader.h"


extern "C" void *_ZN11AbyssEngine16ShaderBaseStructC2Ev(ShaderBaseStruct *self);

// ---- Init_91b6c.cpp ----
namespace AbyssEngine {

void VertexColorAlphaTextureShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("VCATShader.vsh", "VCATShader.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x34) = glGetAttribLocation(program, "a0");
    field_i32(this, 0x20) = glGetAttribLocation(field_i32(this, 0x04), "a1");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a2");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a3");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a4");
    field_i32(this, 0x30) = glGetAttribLocation(field_i32(this, 0x04), "a5");

    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u0");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u1");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u2");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u3");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u5");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u6");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u7");
    field_i32(this, 0x58) = glGetUniformLocation(field_i32(this, 0x04), "u8");

    glUseProgram(field_i32(this, 0x04));
    return glUniform1i(field_i32(this, 0x48), 0);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_91ce4.cpp ----
namespace AbyssEngine {

void VertexColorAlphaTextureShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x38) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x38), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x3c) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x3c), 1, 0, (char *)engine + 0x204);

    if (field_u8(this, 0x9) != 0) {
        if (field_i32(this, 0x40) >= 0)
            glUniform3f(field_i32(this, 0x40), field_f32(engine, 0x330), field_f32(engine, 0x334),
                        field_f32(engine, 0x338));
        if (field_i32(this, 0x44) >= 0)
            glUniform3f(field_i32(this, 0x44), field_f32(engine, 0x34c), field_f32(engine, 0x350),
                        field_f32(engine, 0x354));
        if (field_i32(this, 0x4c) >= 0)
            glUniform4fv(field_i32(this, 0x4c), 1, (float *)((char *)engine + 0xd0));
        if (field_i32(this, 0x50) >= 0)
            glUniform4fv(field_i32(this, 0x50), 1, (float *)((char *)engine + 0x2a8));
        if (field_i32(this, 0x54) >= 0)
            glUniform4fv(field_i32(this, 0x54), 1, (float *)((char *)engine + 0x298));
        if (field_i32(this, 0x58) >= 0)
            glUniform4fv(field_i32(this, 0x58), 1, (float *)((char *)engine + 0x2b8));
        field_u8(this, 0x9) = 0;
    }

    if (field_i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x20));
    if (field_i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x24));
    if (field_i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x28));
    if (field_i32(this, 0x2c) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x2c));
    if (field_i32(this, 0x30) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x30));
    if (field_i32(this, 0x34) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x34));

    if (field_u8(mesh, 0x5c) == 0) {
        if (field_i32(this, 0x20) >= 0)
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (field_i32(this, 0x24) >= 0)
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (field_i32(this, 0x28) >= 0)
            glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        if (field_i32(this, 0x2c) >= 0)
            glVertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        if (field_i32(this, 0x30) >= 0)
            glVertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
        if (field_i32(this, 0x34) >= 0)
            glVertexAttribPointer(field_i32(this, 0x34), 4, 0x1406, 0, 0, field_ptr(mesh, 0xc));
    } else {
        void (*bindBuffer)(uint32_t, uint32_t) = glBindBuffer;
        void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, field_i32(mesh, 0x60));
        vertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x68));
        vertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x6c));
        vertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x70));
        vertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x74));
        vertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x78));
        vertexAttribPointer(field_i32(this, 0x34), 4, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- SetInActive_91c9c.cpp ----
namespace AbyssEngine {

void VertexColorAlphaTextureShader::SetInActive()
{
    int loc;
    loc = field_i32(this, 0x20);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x24);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x28);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x2c);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x30);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x34);
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
// AbyssEngine::VertexColorAlphaTextureShader::VertexColorAlphaTextureShader()
// Chains to the ShaderBaseStruct base ctor, installs this class's vtable, bumps the
// global shader-index counter, and stores the shader's resource name ("VCATShader").

namespace AbyssEngine {

// Base ctor (mangled): ShaderBaseStruct::ShaderBaseStruct()

// Global shader-index counter copied into a destination slot during construction.
__attribute__((visibility("hidden"))) extern int g_shaderIndexSrc;
__attribute__((visibility("hidden"))) extern int g_shaderIndexDst;

VertexColorAlphaTextureShader::VertexColorAlphaTextureShader()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);

    // Install the derived vtable (base address + 8 -> first virtual slot).
    *(void **)this = (void *)(_ZTVN11AbyssEngine29VertexColorAlphaTextureShaderE + 8);

    // Publish the current shader index.
    g_shaderIndexDst = g_shaderIndexSrc;

    // Store the shader resource name in the embedded String at +0x0c.
    String name("VCATShader", false);
    *(String *)((char *)this + 0xc) = name;
}

} // namespace AbyssEngine
