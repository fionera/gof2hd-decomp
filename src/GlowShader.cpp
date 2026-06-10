#include "gof2/GlowShader.h"


extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(AbyssEngine::ShaderBaseStruct *self);
extern "C" void _ZN11AbyssEngine6StringD1Ev(AbyssEngine::String *self);

// ---- SetInActive_92050.cpp ----
namespace AbyssEngine {

void GlowShader::SetInActive()
{
    int loc;
    loc = field_i32(this, 0x20);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x24);
    if (loc < 0)
        return;
    return glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

// ---- _GlowShader_9212a.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

void _ZN11AbyssEngine10GlowShaderD0Ev(
    AbyssEngine::GlowShader *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- UpdateMeshData_92070.cpp ----
namespace AbyssEngine {

void GlowShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x28) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x28), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x2c) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x2c), 1, 0, (char *)engine + 0x204);

    if (field_i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x20));
    if (field_i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x24));

    if (field_u8(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        return glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
    }
    if (field_i32(this, 0x20) >= 0)
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    int index = field_i32(this, 0x24);
    if (index < 0)
        return;
    return glVertexAttribPointer(index, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
}

} // namespace AbyssEngine

// ---- GlowShader_91f38.cpp ----
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringC1EPKcb(
    AbyssEngine::String *self, const char *text, bool copy);
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringaSERKS0_(
    AbyssEngine::String *self, const AbyssEngine::String *other);

namespace AbyssEngine {

struct ConstructorFrame {
    char name[sizeof(String)];
    volatile uint32_t stackGuard;
};

static inline String *shaderName(GlowShader *self)
{
    return (String *)((char *)self + 0x0c);
}

} // namespace AbyssEngine

extern "C" AbyssEngine::GlowShader *
_ZN11AbyssEngine10GlowShaderC2Ev(AbyssEngine::GlowShader *self)
{
    AbyssEngine::ConstructorFrame frame;
    frame.stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;

    _ZN11AbyssEngine16ShaderBaseStructC2Ev((AbyssEngine::ShaderBaseStruct *)self);
    *(void *volatile *)self = _ZTVN11AbyssEngine10GlowShaderE + 8;
    AbyssEngine::GlowShader::ShaderIndex =
        AbyssEngine::ShaderBaseStruct::shaderIndexIntern;

    _ZN11AbyssEngine6StringC1EPKcb(
        (AbyssEngine::String *)frame.name, "GlowShader", false);
    _ZN11AbyssEngine6StringaSERKS0_(
        AbyssEngine::shaderName(self), (AbyssEngine::String *)frame.name);
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)frame.name);

    uint32_t stackDifference = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - frame.stackGuard;
    if (stackDifference != 0) {
        __stack_chk_fail(stackDifference);
    }
    return self;
}

// ---- Init_91fc4.cpp ----
namespace AbyssEngine {

void GlowShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("GlowShader.vsh", "GlowShader.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");

    field_i32(this, 0x28) = glGetUniformLocation(field_i32(this, 0x04), "u_mvp");
    field_i32(this, 0x2c) = glGetUniformLocation(field_i32(this, 0x04), "u_color");
    field_i32(this, 0x30) = glGetUniformLocation(field_i32(this, 0x04), "u_texture");

    glUseProgram(field_i32(this, 0x04));
    return glUniform1i(field_i32(this, 0x30), 0);
}

} // namespace AbyssEngine
