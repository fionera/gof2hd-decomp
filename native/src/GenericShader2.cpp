#include "GenericShader2.h"


extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(AbyssEngine::ShaderBaseStruct *self);
extern "C" void _ZN11AbyssEngine6StringD1Ev(AbyssEngine::String *self);

// ---- _GenericShader2_8e412.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine14GenericShader2D0Ev(
    AbyssEngine::GenericShader2 *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- SetInActive_8e27c.cpp ----
namespace AbyssEngine {

void GenericShader2::SetInActive()
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
    if (loc < 0)
        return;
    return glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

// ---- Init_8e15c.cpp ----
namespace AbyssEngine {

void GenericShader2::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("GenericShader2.vsh", "GenericShader2.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_normal");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a_tangent");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a_binormal");
    field_i32(this, 0x30) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");

    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u_m0");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u_m1");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u_m2");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u_m3");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u_m4");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u_m5");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u_m6");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u_m7");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u_m8");

    glUseProgram(field_i32(this, 0x04));
    return glUniform1i(field_i32(this, 0x44), 0);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8e2ba.cpp ----
namespace AbyssEngine {

void GenericShader2::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x34) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x38) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x38), 1, 0, (char *)engine + 0x204);

    if (field_u8(this, 0x9) != 0) {
        if (field_i32(this, 0x3c) >= 0)
            glUniform3f(field_i32(this, 0x3c), field_f32(engine, 0x330), field_f32(engine, 0x334),
                        field_f32(engine, 0x338));
        if (field_i32(this, 0x40) >= 0)
            glUniform3f(field_i32(this, 0x40), field_f32(engine, 0x34c), field_f32(engine, 0x350),
                        field_f32(engine, 0x354));
        if (field_i32(this, 0x48) >= 0)
            glUniform4fv(field_i32(this, 0x48), 1, (float *)((char *)engine + 0xd0));
        if (field_i32(this, 0x4c) >= 0)
            glUniform4fv(field_i32(this, 0x4c), 1, (float *)((char *)engine + 0x2a8));
        if (field_i32(this, 0x50) >= 0)
            glUniform4fv(field_i32(this, 0x50), 1, (float *)((char *)engine + 0x298));
        if (field_i32(this, 0x54) >= 0)
            glUniform4fv(field_i32(this, 0x54), 1, (float *)((char *)engine + 0x2b8));
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
}

} // namespace AbyssEngine

// ---- GenericShader2_8e0d0.cpp ----
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringC1EPKcb(
    AbyssEngine::String *self, const char *text, bool copy);
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringaSERKS0_(
    AbyssEngine::String *self, const AbyssEngine::String *other);

namespace AbyssEngine {

struct ConstructorFrame {
    char name[sizeof(String)];
    volatile uint32_t stackGuard;
};

static inline String *shaderName(GenericShader2 *self)
{
    return (String *)((char *)self + 0x0c);
}

} // namespace AbyssEngine

extern "C" AbyssEngine::GenericShader2 *
_ZN11AbyssEngine14GenericShader2C2Ev(AbyssEngine::GenericShader2 *self)
{
    AbyssEngine::ConstructorFrame frame;
    frame.stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;

    _ZN11AbyssEngine16ShaderBaseStructC2Ev((AbyssEngine::ShaderBaseStruct *)self);
    *(void *volatile *)self = _ZTVN11AbyssEngine14GenericShader2E + 8;
    AbyssEngine::GenericShader2::ShaderIndex =
        AbyssEngine::ShaderBaseStruct::shaderIndexIntern;

    _ZN11AbyssEngine6StringC1EPKcb(
        (AbyssEngine::String *)frame.name, "GenericShader2", false);
    _ZN11AbyssEngine6StringaSERKS0_(
        AbyssEngine::shaderName(self), (AbyssEngine::String *)frame.name);
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)frame.name);

    uint32_t stackDifference = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - frame.stackGuard;
    if (stackDifference != 0) {
        __stack_chk_fail(stackDifference);
    }
    return self;
}
