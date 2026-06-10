#include "EnergyShield.h"


extern "C" void glUniform2f(int location, float x, float y);
extern "C" int _ZN11AbyssEngine6Engine15GetDisplayWidthEv(AbyssEngine::Engine *engine);
extern "C" int _ZN11AbyssEngine6Engine16GetDisplayHeightEv(AbyssEngine::Engine *engine);
extern "C" void _ZN11AbyssEngine6Engine17ActivateRefractFBOEv(AbyssEngine::Engine *engine);
extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(AbyssEngine::ShaderBaseStruct *self);
extern "C" void _ZN11AbyssEngine6StringD1Ev(AbyssEngine::String *self);

// ---- _EnergyShield_8a5d0.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine12EnergyShieldD0Ev(
    AbyssEngine::EnergyShield *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- UpdateMeshData_8a40c.cpp ----
namespace AbyssEngine {

void EnergyShield::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x2c) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x2c), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x30) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x30), 1, 0, (char *)engine + 0x184);

    if (field_u8(this, 0x9) != 0) {
        if (field_i32(this, 0x34) >= 0)
            glUniform3f(field_i32(this, 0x34), field_f32(engine, 0x34c), field_f32(engine, 0x350),
                        field_f32(engine, 0x354));
        if (field_i32(this, 0x44) >= 0)
            glUniform4fv(field_i32(this, 0x44), 1, (float *)((char *)engine + 0xd0));
        if (field_i32(this, 0x48) >= 0)
            glUniform3fv(field_i32(this, 0x48), 1, (float *)((char *)engine + 0x320));
        int loc = field_i32(this, 0x58);
        if (loc >= 0) {
            float w = (float)_ZN11AbyssEngine6Engine15GetDisplayWidthEv(engine);
            float h = (float)_ZN11AbyssEngine6Engine16GetDisplayHeightEv(engine);
            glUniform2f(loc, 1.0f / w, 1.0f / h);
        }
        glActiveTexture(0x84c7);
        _ZN11AbyssEngine6Engine17ActivateRefractFBOEv(engine);
        glUniform1f(field_i32(this, 0x50), field_f32(mesh, 0x24));
        glUniform1f(field_i32(this, 0x54), field_f32(mesh, 0x24) / 3.0f);
        field_u8(this, 0x9) = 0;
    }

    if (field_i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x20));
    if (field_i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x24));
    if (field_i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x28));

    if (field_u8(mesh, 0x5c) == 0) {
        if (field_i32(this, 0x20) >= 0)
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (field_i32(this, 0x24) >= 0)
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (field_i32(this, 0x28) >= 0)
            glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- SetInActive_8a3e0.cpp ----
namespace AbyssEngine {

void EnergyShield::SetInActive()
{
    int loc;
    loc = field_i32(this, 0x20);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x24);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x28);
    if (loc < 0)
        return;
    return glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

// ---- Init_8a28c.cpp ----
namespace AbyssEngine {

void EnergyShield::Init(Engine *engine)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("EnergyShield.vsh", "EnergyShield.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");

    field_i32(this, 0x28) = glGetUniformLocation(field_i32(this, 0x04), "u_m0");
    field_i32(this, 0x2c) = glGetUniformLocation(field_i32(this, 0x04), "u_m1");
    field_i32(this, 0x30) = glGetUniformLocation(field_i32(this, 0x04), "u_m2");
    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u_m3");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u_tex0");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u_tex1");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u_m4");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u_m5");
    field_i32(this, 0x58) = glGetUniformLocation(field_i32(this, 0x04), "u_m6");

    glActiveTexture(0x84c7);
    _ZN11AbyssEngine6Engine17ActivateRefractFBOEv(engine);

    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u_refract");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u_m7");
    field_i32(this, 0x5c) = glGetUniformLocation(field_i32(this, 0x04), "u_m8");

    glUseProgram(field_i32(this, 0x04));

    for (int i = 0; i != 2; i = i + 1) {
        if (field_i32(this, 0x38 + i * 4) >= 0)
            glUniform1i(field_i32(this, 0x38 + i * 4), i);
    }
    return glUniform1i(field_i32(this, 0x5c), 7);
}

} // namespace AbyssEngine

// ---- EnergyShield_8a200.cpp ----
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringC1EPKcb(
    AbyssEngine::String *self, const char *text, bool copy);
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringaSERKS0_(
    AbyssEngine::String *self, const AbyssEngine::String *other);

namespace AbyssEngine {

struct ConstructorFrame {
    char name[sizeof(String)];
    volatile uint32_t stackGuard;
};

static inline String *shaderName(EnergyShield *self)
{
    return (String *)((char *)self + 0x0c);
}

} // namespace AbyssEngine

extern "C" AbyssEngine::EnergyShield *
_ZN11AbyssEngine12EnergyShieldC2Ev(AbyssEngine::EnergyShield *self)
{
    AbyssEngine::ConstructorFrame frame;
    frame.stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;

    _ZN11AbyssEngine16ShaderBaseStructC2Ev((AbyssEngine::ShaderBaseStruct *)self);
    *(void *volatile *)self = _ZTVN11AbyssEngine12EnergyShieldE + 8;
    AbyssEngine::EnergyShield::ShaderIndex =
        AbyssEngine::ShaderBaseStruct::shaderIndexIntern;

    _ZN11AbyssEngine6StringC1EPKcb(
        (AbyssEngine::String *)frame.name, "EnergyShield", false);
    _ZN11AbyssEngine6StringaSERKS0_(
        AbyssEngine::shaderName(self), (AbyssEngine::String *)frame.name);
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)frame.name);

    uint32_t stackDifference = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - frame.stackGuard;
    if (stackDifference != 0) {
        __stack_chk_fail(stackDifference);
    }
    return self;
}
