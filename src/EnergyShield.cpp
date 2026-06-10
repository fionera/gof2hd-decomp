#include "gof2/EnergyShield.h"


extern "C" void glUniform2f(int location, float x, float y);
extern "C" int _ZN11AbyssEngine6Engine15GetDisplayWidthEv(AbyssEngine::Engine *engine);
extern "C" int _ZN11AbyssEngine6Engine16GetDisplayHeightEv(AbyssEngine::Engine *engine);
extern "C" void _ZN11AbyssEngine6Engine17ActivateRefractFBOEv(AbyssEngine::Engine *engine);
extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(AbyssEngine::ShaderBaseStruct *self);
extern "C" void _ZN11AbyssEngine6StringD1Ev(AbyssEngine::String *self);

// ---- _EnergyShield_8a5d0.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

void _ZN11AbyssEngine12EnergyShieldD0Ev(
    AbyssEngine::EnergyShield *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- UpdateMeshData_8a40c.cpp ----
namespace AbyssEngine {

void EnergyShield::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->field_0x2c >= 0)
        glUniformMatrix4fv(this->field_0x2c, 1, 0, (char *)engine + 0x104);
    if (this->field_0x30 >= 0)
        glUniformMatrix4fv(this->field_0x30, 1, 0, (char *)engine + 0x184);

    if (this->field_0x9 != 0) {
        if (this->field_0x34 >= 0)
            glUniform3f(this->field_0x34, field_f32(engine, 0x34c), field_f32(engine, 0x350),
                        field_f32(engine, 0x354));
        if (this->field_0x44 >= 0)
            glUniform4fv(this->field_0x44, 1, (float *)((char *)engine + 0xd0));
        if (this->field_0x48 >= 0)
            glUniform3fv(this->field_0x48, 1, (float *)((char *)engine + 0x320));
        int loc = this->field_0x58;
        if (loc >= 0) {
            float w = (float)_ZN11AbyssEngine6Engine15GetDisplayWidthEv(engine);
            float h = (float)_ZN11AbyssEngine6Engine16GetDisplayHeightEv(engine);
            glUniform2f(loc, 1.0f / w, 1.0f / h);
        }
        glActiveTexture(0x84c7);
        _ZN11AbyssEngine6Engine17ActivateRefractFBOEv(engine);
        glUniform1f(this->field_0x50, field_f32(mesh, 0x24));
        glUniform1f(this->field_0x54, field_f32(mesh, 0x24) / 3.0f);
        this->field_0x9 = 0;
    }

    if (this->field_0x20 >= 0)
        glEnableVertexAttribArray(this->field_0x20);
    if (this->field_0x24 >= 0)
        glEnableVertexAttribArray(this->field_0x24);
    if (this->field_0x28 >= 0)
        glEnableVertexAttribArray(this->field_0x28);

    if (field_u8(mesh, 0x5c) == 0) {
        if (this->field_0x20 >= 0)
            glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (this->field_0x24 >= 0)
            glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (this->field_0x28 >= 0)
            glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- SetInActive_8a3e0.cpp ----
namespace AbyssEngine {

void EnergyShield::SetInActive()
{
    int loc;
    loc = this->field_0x20;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x24;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x28;
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
    this->field_0x4 = program;

    this->field_0x20 = glGetAttribLocation(program, "a_position");
    this->field_0x24 = glGetAttribLocation(this->field_0x4, "a_texCoord");

    this->field_0x28 = glGetUniformLocation(this->field_0x4, "u_m0");
    this->field_0x2c = glGetUniformLocation(this->field_0x4, "u_m1");
    this->field_0x30 = glGetUniformLocation(this->field_0x4, "u_m2");
    this->field_0x34 = glGetUniformLocation(this->field_0x4, "u_m3");
    this->field_0x38 = glGetUniformLocation(this->field_0x4, "u_tex0");
    this->field_0x3c = glGetUniformLocation(this->field_0x4, "u_tex1");
    this->field_0x44 = glGetUniformLocation(this->field_0x4, "u_m4");
    this->field_0x48 = glGetUniformLocation(this->field_0x4, "u_m5");
    this->field_0x58 = glGetUniformLocation(this->field_0x4, "u_m6");

    glActiveTexture(0x84c7);
    _ZN11AbyssEngine6Engine17ActivateRefractFBOEv(engine);

    this->field_0x54 = glGetUniformLocation(this->field_0x4, "u_refract");
    this->field_0x50 = glGetUniformLocation(this->field_0x4, "u_m7");
    this->field_0x5c = glGetUniformLocation(this->field_0x4, "u_m8");

    glUseProgram(this->field_0x4);

    for (int i = 0; i != 2; i = i + 1) {
        if ((&this->field_0x38)[i] >= 0)
            glUniform1i((&this->field_0x38)[i], i);
    }
    return glUniform1i(this->field_0x5c, 7);
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
    return &self->field_0xc;
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
