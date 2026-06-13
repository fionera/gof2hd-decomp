#include "gof2/engine/render/shaders/DNSShader.h"

extern "C" float DNSShader_g0;
extern "C" float DNSShader_g1;

void _ZN11AbyssEngine9DNSShaderD0Ev(
    AbyssEngine::DNSShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

namespace AbyssEngine {

void DNSShader::SetInActive()
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

namespace AbyssEngine {

void DNSShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x34) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x38) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x38), 1, 0, (char *)engine + 0x204);
    if (field_i32(this, 0x3c) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x3c), 1, 0, (char *)engine + 0x144);
    if (field_i32(this, 0x5c) >= 0)
        glUniform1f(field_i32(this, 0x5c), DNSShader_g0);
    if (field_i32(this, 0x60) >= 0)
        glUniform1f(field_i32(this, 0x60), DNSShader_g1);

    if (field_u8(this, 0x9) != 0) {
        glUniform3f(field_i32(this, 0x40), field_f32(engine, 0x330), field_f32(engine, 0x334),
                    field_f32(engine, 0x338));
        if (field_i32(this, 0x44) >= 0)
            glUniform3f(field_i32(this, 0x44), field_f32(engine, 0x34c), field_f32(engine, 0x350),
                        field_f32(engine, 0x354));
        if (field_i32(this, 0x48) >= 0)
            glUniform4fv(field_i32(this, 0x48), 1, (float *)((char *)engine + 0xd0));
        if (field_i32(this, 0x4c) >= 0)
            glUniform3fv(field_i32(this, 0x4c), 1, (float *)((char *)engine + 0x2cc));
        if (field_i32(this, 0x50) >= 0)
            glUniform3fv(field_i32(this, 0x50), 1, (float *)((char *)engine + 0x2fc));
        if (field_i32(this, 0x54) >= 0)
            glUniform3fv(field_i32(this, 0x54), 1, (float *)((char *)engine + 0x2e4));
        if (field_i32(this, 0x58) >= 0)
            glUniform1f(field_i32(this, 0x58), field_f32(engine, 0x2c8));
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
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x70));
        glVertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x74));
        glVertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

namespace AbyssEngine {

void DNSShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->LoadBindShader("DNSShader.vsh", "DNSShader.fsh");
    field_i32(this, 0x04) = program;
    if (program == 0) {
        program = ((ShaderBaseStruct *)this)->ES2LoadProgram("DNSShader.vsh", "DNSShader.fsh");
        field_i32(this, 0x04) = program;
    }

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_normal");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a_tangent");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a_binormal");

    field_i32(this, 0x30) = glGetUniformLocation(field_i32(this, 0x04), "u_m0");
    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u_m1");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u_m2");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u_m3");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u_m4");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u_m5");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u_m6");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u_m7");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u_m8");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u_m9");
    field_i32(this, 0x58) = glGetUniformLocation(field_i32(this, 0x04), "u_m10");
    field_i32(this, 0x5c) = glGetUniformLocation(field_i32(this, 0x04), "u_m11");
    field_i32(this, 0x60) = glGetUniformLocation(field_i32(this, 0x04), "u_m12");

    return glUseProgram(field_i32(this, 0x04));
}

} // namespace AbyssEngine

extern "C" AbyssEngine::DNSShader *
_ZN11AbyssEngine9DNSShaderC2Ev(AbyssEngine::DNSShader *self)
{
    new ((AbyssEngine::ShaderBaseStruct *)self) AbyssEngine::ShaderBaseStruct();
    *(void *volatile *)self = _ZTVN11AbyssEngine9DNSShaderE + 8;
    AbyssEngine::DNSShader::ShaderIndex =
        AbyssEngine::ShaderBaseStruct::shaderIndexIntern;

    String name("DNSShader");
    self->shaderName.assign(&name);
    return self;
}
