#include "gof2/NoTexShader.h"


extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(AbyssEngine::ShaderBaseStruct *self);
extern "C" void _ZN11AbyssEngine6StringD1Ev(AbyssEngine::String *self);

// ---- SetInActive_906b0.cpp ----
namespace AbyssEngine {

void NoTexShader::SetInActive()
{
    return glDisableVertexAttribArray(field_i32(this, 0x20));
}

} // namespace AbyssEngine

// ---- _NoTexShader_9073a.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine11NoTexShaderD0Ev(
    AbyssEngine::NoTexShader *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- NoTexShader_905c8.cpp ----
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringC1EPKcb(
    AbyssEngine::String *self, const char *text, bool copy);
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringaSERKS0_(
    AbyssEngine::String *self, const AbyssEngine::String *other);

namespace AbyssEngine {

struct ConstructorFrame {
    char name[sizeof(String)];
    volatile uint32_t stackGuard;
};

static inline String *shaderName(NoTexShader *self)
{
    return (String *)((char *)self + 0x0c);
}

} // namespace AbyssEngine

extern "C" AbyssEngine::NoTexShader *
_ZN11AbyssEngine11NoTexShaderC2Ev(AbyssEngine::NoTexShader *self)
{
    AbyssEngine::ConstructorFrame frame;
    frame.stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;

    _ZN11AbyssEngine16ShaderBaseStructC2Ev((AbyssEngine::ShaderBaseStruct *)self);
    *(void *volatile *)self = _ZTVN11AbyssEngine11NoTexShaderE + 8;
    AbyssEngine::NoTexShader::ShaderIndex =
        AbyssEngine::ShaderBaseStruct::shaderIndexIntern;

    _ZN11AbyssEngine6StringC1EPKcb(
        (AbyssEngine::String *)frame.name, "NoTexShader", false);
    _ZN11AbyssEngine6StringaSERKS0_(
        AbyssEngine::shaderName(self), (AbyssEngine::String *)frame.name);
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)frame.name);

    uint32_t stackDifference = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - frame.stackGuard;
    if (stackDifference != 0) {
        __stack_chk_fail(stackDifference);
    }
    return self;
}

// ---- UpdateMeshData_906b6.cpp ----
namespace AbyssEngine {

void NoTexShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_i32(this, 0x24), 1, 0, (char *)engine + 0x104);
    if (field_u8(this, 0x9) != 0) {
        glUniform4fv(field_i32(this, 0x28), 1, (float *)((char *)engine + 0xd0));
        field_u8(this, 0x9) = 0;
    }
    glEnableVertexAttribArray(field_i32(this, 0x20));

    int index, size;
    const void *ptr;
    if (mesh == 0) {
        ptr = field_ptr(engine, 0x348);
        index = field_i32(this, 0x20);
        size = 2;
    } else {
        if (field_u8(mesh, 0x5c) == 0) {
            index = field_i32(this, 0x20);
            ptr = field_ptr(mesh, 0x4);
        } else {
            glBindBuffer(0x8892, field_i32(mesh, 0x60));
            ptr = 0;
            index = field_i32(this, 0x20);
        }
        size = 3;
    }
    return glVertexAttribPointer(index, size, 0x1406, 0, 0, ptr);
}

} // namespace AbyssEngine

// ---- Init_90654.cpp ----
namespace AbyssEngine {

void NoTexShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("NoTexShader.vsh", "NoTexShader.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");

    field_i32(this, 0x24) = glGetUniformLocation(field_i32(this, 0x04), "u_mvp");
    field_i32(this, 0x28) = glGetUniformLocation(field_i32(this, 0x04), "u_color");

    return glUseProgram(field_i32(this, 0x04));
}

} // namespace AbyssEngine
