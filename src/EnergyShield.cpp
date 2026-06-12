#include "gof2/EnergyShield.h"


extern "C" void glUniform2f(int location, float x, float y);
extern "C" int _ZN11AbyssEngine6Engine15GetDisplayWidthEv(AbyssEngine::Engine *engine);
extern "C" int _ZN11AbyssEngine6Engine16GetDisplayHeightEv(AbyssEngine::Engine *engine);
extern "C" void _ZN11AbyssEngine6Engine17ActivateRefractFBOEv(AbyssEngine::Engine *engine);
extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(AbyssEngine::ShaderBaseStruct *self);

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
    if (this->uM1 >= 0)
        glUniformMatrix4fv(this->uM1, 1, 0, (char *)engine + 0x104);
    if (this->uM2 >= 0)
        glUniformMatrix4fv(this->uM2, 1, 0, (char *)engine + 0x184);

    if (this->needsUniformUpdate != 0) {
        if (this->uM3 >= 0)
            glUniform3f(this->uM3, field_f32(engine, 0x34c), field_f32(engine, 0x350),
                        field_f32(engine, 0x354));
        if (this->uM4 >= 0)
            glUniform4fv(this->uM4, 1, (float *)((char *)engine + 0xd0));
        if (this->uM5 >= 0)
            glUniform3fv(this->uM5, 1, (float *)((char *)engine + 0x320));
        int loc = this->uM6;
        if (loc >= 0) {
            float w = (float)_ZN11AbyssEngine6Engine15GetDisplayWidthEv(engine);
            float h = (float)_ZN11AbyssEngine6Engine16GetDisplayHeightEv(engine);
            glUniform2f(loc, 1.0f / w, 1.0f / h);
        }
        glActiveTexture(0x84c7);
        _ZN11AbyssEngine6Engine17ActivateRefractFBOEv(engine);
        glUniform1f(this->uM7, field_f32(mesh, 0x24));
        glUniform1f(this->uRefract, field_f32(mesh, 0x24) / 3.0f);
        this->needsUniformUpdate = 0;
    }

    if (this->aPosition >= 0)
        glEnableVertexAttribArray(this->aPosition);
    if (this->aTexCoord >= 0)
        glEnableVertexAttribArray(this->aTexCoord);
    if (this->uM0 >= 0)
        glEnableVertexAttribArray(this->uM0);

    if (field_u8(mesh, 0x5c) == 0) {
        if (this->aPosition >= 0)
            glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (this->aTexCoord >= 0)
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (this->uM0 >= 0)
            glVertexAttribPointer(this->uM0, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(this->uM0, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- SetInActive_8a3e0.cpp ----
namespace AbyssEngine {

void EnergyShield::SetInActive()
{
    int loc;
    loc = this->aPosition;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->aTexCoord;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->uM0;
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
    this->program = program;

    this->aPosition = glGetAttribLocation(program, "a_position");
    this->aTexCoord = glGetAttribLocation(this->program, "a_texCoord");

    this->uM0 = glGetUniformLocation(this->program, "u_m0");
    this->uM1 = glGetUniformLocation(this->program, "u_m1");
    this->uM2 = glGetUniformLocation(this->program, "u_m2");
    this->uM3 = glGetUniformLocation(this->program, "u_m3");
    this->uTex0 = glGetUniformLocation(this->program, "u_tex0");
    this->uTex1 = glGetUniformLocation(this->program, "u_tex1");
    this->uM4 = glGetUniformLocation(this->program, "u_m4");
    this->uM5 = glGetUniformLocation(this->program, "u_m5");
    this->uM6 = glGetUniformLocation(this->program, "u_m6");

    glActiveTexture(0x84c7);
    _ZN11AbyssEngine6Engine17ActivateRefractFBOEv(engine);

    this->uRefract = glGetUniformLocation(this->program, "u_refract");
    this->uM7 = glGetUniformLocation(this->program, "u_m7");
    this->uM8 = glGetUniformLocation(this->program, "u_m8");

    glUseProgram(this->program);

    for (int i = 0; i != 2; i = i + 1) {
        if ((&this->uTex0)[i] >= 0)
            glUniform1i((&this->uTex0)[i], i);
    }
    return glUniform1i(this->uM8, 7);
}

} // namespace AbyssEngine

// ---- EnergyShield_8a200.cpp ----
extern "C" AbyssEngine::EnergyShield *
_ZN11AbyssEngine12EnergyShieldC2Ev(AbyssEngine::EnergyShield *self)
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((AbyssEngine::ShaderBaseStruct *)self);
    *(void *volatile *)self = _ZTVN11AbyssEngine12EnergyShieldE + 8;
    AbyssEngine::EnergyShield::ShaderIndex =
        AbyssEngine::ShaderBaseStruct::shaderIndexIntern;

    String name("EnergyShield");
    self->shaderName.assign(&name);
    return self;
}
