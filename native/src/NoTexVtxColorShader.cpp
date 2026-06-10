#include "NoTexVtxColorShader.h"


extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(AbyssEngine::ShaderBaseStruct *self);
extern "C" void _ZN11AbyssEngine6StringD1Ev(AbyssEngine::String *self);

// ---- SetInActive_8e024.cpp ----
namespace AbyssEngine {

void NoTexVtxColorShader::SetInActive()
{
    glDisableVertexAttribArray(field_i32(this, 0x20));
    return glDisableVertexAttribArray(field_i32(this, 0x24));
}

} // namespace AbyssEngine

// ---- _NoTexVtxColorShader_8e0be.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine19NoTexVtxColorShaderD0Ev(
    AbyssEngine::NoTexVtxColorShader *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- Init_8dfb8.cpp ----
namespace AbyssEngine {

void NoTexVtxColorShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram(
        "NoTexVtxColorShader.vsh", "NoTexVtxColorShader.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_color");

    field_i32(this, 0x28) = glGetUniformLocation(field_i32(this, 0x04), "u_mvp");
    field_i32(this, 0x2c) = glGetUniformLocation(field_i32(this, 0x04), "u_color");

    return glUseProgram(field_i32(this, 0x04));
}

} // namespace AbyssEngine

// ---- NoTexVtxColorShader_8df2c.cpp ----
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringC1EPKcb(
    AbyssEngine::String *self, const char *text, bool copy);
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringaSERKS0_(
    AbyssEngine::String *self, const AbyssEngine::String *other);

namespace AbyssEngine {

struct ConstructorFrame {
    char name[sizeof(String)];
    volatile uint32_t stackGuard;
};

static inline String *shaderName(NoTexVtxColorShader *self)
{
    return (String *)((char *)self + 0x0c);
}

} // namespace AbyssEngine

extern "C" AbyssEngine::NoTexVtxColorShader *
_ZN11AbyssEngine19NoTexVtxColorShaderC2Ev(AbyssEngine::NoTexVtxColorShader *self)
{
    AbyssEngine::ConstructorFrame frame;
    frame.stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;

    _ZN11AbyssEngine16ShaderBaseStructC2Ev((AbyssEngine::ShaderBaseStruct *)self);
    *(void *volatile *)self = _ZTVN11AbyssEngine19NoTexVtxColorShaderE + 8;
    AbyssEngine::NoTexVtxColorShader::ShaderIndex =
        AbyssEngine::ShaderBaseStruct::shaderIndexIntern;

    _ZN11AbyssEngine6StringC1EPKcb(
        (AbyssEngine::String *)frame.name, "NoTexVtxColorShader", false);
    _ZN11AbyssEngine6StringaSERKS0_(
        AbyssEngine::shaderName(self), (AbyssEngine::String *)frame.name);
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)frame.name);

    uint32_t stackDifference = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - frame.stackGuard;
    if (stackDifference != 0) {
        __stack_chk_fail(stackDifference);
    }
    return self;
}

// ---- UpdateMeshData_8e03a.cpp ----
namespace AbyssEngine {

void NoTexVtxColorShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_i32(this, 0x28), 1, 0, (char *)engine + 0x104);
    if (field_u8(this, 0x9) != 0) {
        glUniform4fv(field_i32(this, 0x2c), 1, (float *)((char *)engine + 0xd0));
        field_u8(this, 0x9) = 0;
    }
    glEnableVertexAttribArray(field_i32(this, 0x20));
    glEnableVertexAttribArray(field_i32(this, 0x24));

    int index = field_i32(this, 0x20);
    int size;
    const void *ptr;
    int stride = 0;
    if (mesh == 0) {
        ptr = field_ptr(engine, 0x348);
        size = 2;
    } else {
        glVertexAttribPointer(index, 3, 0x1406, 0, stride, field_ptr(mesh, 0x4));
        ptr = field_ptr(mesh, 0xc);
        index = field_i32(this, 0x24);
        size = 4;
    }
    return glVertexAttribPointer(index, size, 0x1406, 0, stride, ptr);
}

} // namespace AbyssEngine
