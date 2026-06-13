#include "gof2/engine/render/shaders/DNSShader.h"
#include "gof2/game/core/String.h"

// PC-relative globals holding single floats (engine timers) read by UpdateMeshData.
extern "C" float DNSShader_g0;
extern "C" float DNSShader_g1;

void _ZN11AbyssEngine9DNSShaderD0Ev(
    AbyssEngine::DNSShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    ::operator delete(base);
}

namespace AbyssEngine {

void DNSShader::SetInActive()
{
    if (this->aPositionLoc >= 0)
        glDisableVertexAttribArray(this->aPositionLoc);
    if (this->aNormalLoc >= 0)
        glDisableVertexAttribArray(this->aNormalLoc);
    if (this->aTangentLoc >= 0)
        glDisableVertexAttribArray(this->aTangentLoc);
    if (this->aBinormalLoc >= 0)
        glDisableVertexAttribArray(this->aBinormalLoc);
    if (this->uM0Loc < 0)
        return;
    return glDisableVertexAttribArray(this->uM0Loc);
}

} // namespace AbyssEngine

namespace AbyssEngine {

void DNSShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->uM1Loc >= 0)
        glUniformMatrix4fv(this->uM1Loc, 1, 0, (char *)engine + 0x104);
    if (this->uM2Loc >= 0)
        glUniformMatrix3fv(this->uM2Loc, 1, 0, (char *)engine + 0x204);
    if (this->uM3Loc >= 0)
        glUniformMatrix4fv(this->uM3Loc, 1, 0, (char *)engine + 0x144);
    if (this->uM11Loc >= 0)
        glUniform1f(this->uM11Loc, DNSShader_g0);
    if (this->uM12Loc >= 0)
        glUniform1f(this->uM12Loc, DNSShader_g1);

    if (this->dirty != 0) {
        glUniform3f(this->uM4Loc, field_f32(engine, 0x330), field_f32(engine, 0x334),
                    field_f32(engine, 0x338));
        if (this->uM5Loc >= 0)
            glUniform3f(this->uM5Loc, field_f32(engine, 0x34c), field_f32(engine, 0x350),
                        field_f32(engine, 0x354));
        if (this->uM6Loc >= 0)
            glUniform4fv(this->uM6Loc, 1, (float *)((char *)engine + 0xd0));
        if (this->uM7Loc >= 0)
            glUniform3fv(this->uM7Loc, 1, (float *)((char *)engine + 0x2cc));
        if (this->uM8Loc >= 0)
            glUniform3fv(this->uM8Loc, 1, (float *)((char *)engine + 0x2fc));
        if (this->uM9Loc >= 0)
            glUniform3fv(this->uM9Loc, 1, (float *)((char *)engine + 0x2e4));
        if (this->uM10Loc >= 0)
            glUniform1f(this->uM10Loc, field_f32(engine, 0x2c8));
        this->dirty = 0;
    }

    if (this->aPositionLoc >= 0)
        glEnableVertexAttribArray(this->aPositionLoc);
    if (this->aNormalLoc >= 0)
        glEnableVertexAttribArray(this->aNormalLoc);
    if (this->aTangentLoc >= 0)
        glEnableVertexAttribArray(this->aTangentLoc);
    if (this->aBinormalLoc >= 0)
        glEnableVertexAttribArray(this->aBinormalLoc);
    if (this->uM0Loc >= 0)
        glEnableVertexAttribArray(this->uM0Loc);

    if (field_u8(mesh, 0x5c) == 0) {
        if (this->aPositionLoc >= 0)
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (this->aNormalLoc >= 0)
            glVertexAttribPointer(this->aNormalLoc, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (this->aTangentLoc >= 0)
            glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        if (this->aBinormalLoc >= 0)
            glVertexAttribPointer(this->aBinormalLoc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        if (this->uM0Loc >= 0)
            glVertexAttribPointer(this->uM0Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(this->aNormalLoc, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x70));
        glVertexAttribPointer(this->aBinormalLoc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x74));
        glVertexAttribPointer(this->uM0Loc, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

namespace AbyssEngine {

void DNSShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("DNSShader.vsh", "DNSShader.fsh");
    this->field_0x4 = program;

    this->aPositionLoc = glGetAttribLocation(program, "a_position");
    this->aNormalLoc = glGetAttribLocation(this->field_0x4, "a_normal");
    this->aTangentLoc = glGetAttribLocation(this->field_0x4, "a_tangent");
    this->aBinormalLoc = glGetAttribLocation(this->field_0x4, "a_binormal");

    this->uM0Loc = glGetUniformLocation(this->field_0x4, "u_m0");
    this->uM1Loc = glGetUniformLocation(this->field_0x4, "u_m1");
    this->uM2Loc = glGetUniformLocation(this->field_0x4, "u_m2");
    this->uM3Loc = glGetUniformLocation(this->field_0x4, "u_m3");
    this->uM4Loc = glGetUniformLocation(this->field_0x4, "u_m4");
    this->uM5Loc = glGetUniformLocation(this->field_0x4, "u_m5");
    this->uM6Loc = glGetUniformLocation(this->field_0x4, "u_m6");
    this->uM7Loc = glGetUniformLocation(this->field_0x4, "u_m7");
    this->uM8Loc = glGetUniformLocation(this->field_0x4, "u_m8");
    this->uM9Loc = glGetUniformLocation(this->field_0x4, "u_m9");
    this->uM10Loc = glGetUniformLocation(this->field_0x4, "u_m10");
    this->uM11Loc = glGetUniformLocation(this->field_0x4, "u_m11");
    this->uM12Loc = glGetUniformLocation(this->field_0x4, "u_m12");

    return glUseProgram(this->field_0x4);
}

} // namespace AbyssEngine

namespace AbyssEngine {

DNSShader::DNSShader()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    this->field_0x0 = (void *)(DNSShader_vtable + 8);
    this->name = DNSShader_name;
}

} // namespace AbyssEngine
