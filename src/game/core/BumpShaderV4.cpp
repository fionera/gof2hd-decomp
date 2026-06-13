#include "gof2/game/core/BumpShaderV4.h"




// ---- SetInActive_8b3e0.cpp ----
namespace AbyssEngine {

void BumpShaderV4::SetInActive()
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
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8b420.cpp ----
namespace AbyssEngine {

void BumpShaderV4::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x34) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x38) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x38), 1, 0, (char *)engine + 0x204);

    if (field_u8(this, 0x9) != 0) {
        if (field_i32(this, 0x3c) >= 0)
            glUniform3f(field_i32(this, 0x3c), field_f32(engine, 0x330),
                        field_f32(engine, 0x334), field_f32(engine, 0x338));
        if (field_i32(this, 0x40) >= 0)
            glUniform3f(field_i32(this, 0x40), field_f32(engine, 0x34c),
                        field_f32(engine, 0x350), field_f32(engine, 0x354));
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

// ---- _BumpShaderV4_8b624.cpp ----
void _ZN11AbyssEngine12BumpShaderV4D0Ev(
    AbyssEngine::BumpShaderV4 *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

// ---- Init_8b29c.cpp ----
namespace AbyssEngine {

void BumpShaderV4::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpShaderV4.vsh", "BumpShaderV4.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a0");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a1");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a2");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a3");
    field_i32(this, 0x30) = glGetAttribLocation(field_i32(this, 0x04), "a4");

    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u0");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u1");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u2");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u3");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u5");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u6");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u7");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u8");
    field_i32(this, 0x58) = glGetUniformLocation(field_i32(this, 0x04), "u9");

    glUseProgram(field_i32(this, 0x04));
    for (int i = 0; i != 2; i++) {
        int loc = field_i32(this, i * 4 + 0x44);
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

} // namespace AbyssEngine

// ---- BumpShaderV4_8b210.cpp ----
namespace AbyssEngine {

int BumpShaderV4::ShaderIndex;

__attribute__((minsize)) BumpShaderV4::BumpShaderV4()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    *(void **)this = (void *)(_ZTVN11AbyssEngine12BumpShaderV4E + 8);
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;

    String tmp("BumpShaderV4");
    this->field_0xc().assign(&tmp);
}

} // namespace AbyssEngine
