#include "gof2/game/core/BumpShaderV4.h"

namespace AbyssEngine {

void BumpShaderV4::SetInActive()
{
    int loc;
    loc = this->field_0x20;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x24;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x28;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x2c;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x30;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

namespace AbyssEngine {

void BumpShaderV4::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->field_0x34 >= 0)
        glUniformMatrix4fv(this->field_0x34, 1, 0, (char *)engine + 0x104);
    if (this->field_0x38 >= 0)
        glUniformMatrix3fv(this->field_0x38, 1, 0, (char *)engine + 0x204);

    if (this->field_0x9 != 0) {
        if (this->field_0x3c >= 0)
            glUniform3f(this->field_0x3c, field_f32(engine, 0x330),
                        field_f32(engine, 0x334), field_f32(engine, 0x338));
        if (this->field_0x40 >= 0)
            glUniform3f(this->field_0x40, field_f32(engine, 0x34c),
                        field_f32(engine, 0x350), field_f32(engine, 0x354));
        if (this->field_0x4c >= 0)
            glUniform4fv(this->field_0x4c, 1, (float *)((char *)engine + 0xd0));
        if (this->field_0x50 >= 0)
            glUniform4fv(this->field_0x50, 1, (float *)((char *)engine + 0x2a8));
        if (this->field_0x54 >= 0)
            glUniform4fv(this->field_0x54, 1, (float *)((char *)engine + 0x298));
        if (this->field_0x58 >= 0)
            glUniform4fv(this->field_0x58, 1, (float *)((char *)engine + 0x2b8));
        this->field_0x9 = 0;
    }

    if (this->field_0x20 >= 0)
        glEnableVertexAttribArray(this->field_0x20);
    if (this->field_0x24 >= 0)
        glEnableVertexAttribArray(this->field_0x24);
    if (this->field_0x28 >= 0)
        glEnableVertexAttribArray(this->field_0x28);
    if (this->field_0x2c >= 0)
        glEnableVertexAttribArray(this->field_0x2c);
    if (this->field_0x30 >= 0)
        glEnableVertexAttribArray(this->field_0x30);

    if (field_u8(mesh, 0x5c) == 0) {
        if (this->field_0x20 >= 0)
            glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (this->field_0x24 >= 0)
            glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (this->field_0x28 >= 0)
            glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        if (this->field_0x2c >= 0)
            glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        if (this->field_0x30 >= 0)
            glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x70));
        glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x74));
        glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

void _ZN11AbyssEngine12BumpShaderV4D0Ev(
    AbyssEngine::BumpShaderV4 *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

namespace AbyssEngine {

void BumpShaderV4::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpShaderV4.vsh", "BumpShaderV4.fsh");
    this->field_0x4 = program;

    this->field_0x20 = glGetAttribLocation(program, "a0");
    this->field_0x24 = glGetAttribLocation(this->field_0x4, "a1");
    this->field_0x28 = glGetAttribLocation(this->field_0x4, "a2");
    this->field_0x2c = glGetAttribLocation(this->field_0x4, "a3");
    this->field_0x30 = glGetAttribLocation(this->field_0x4, "a4");

    this->field_0x34 = glGetUniformLocation(this->field_0x4, "u0");
    this->field_0x38 = glGetUniformLocation(this->field_0x4, "u1");
    this->field_0x3c = glGetUniformLocation(this->field_0x4, "u2");
    this->field_0x40 = glGetUniformLocation(this->field_0x4, "u3");
    this->field_0x44 = glGetUniformLocation(this->field_0x4, "u4");
    this->field_0x48 = glGetUniformLocation(this->field_0x4, "u5");
    this->field_0x4c = glGetUniformLocation(this->field_0x4, "u6");
    this->field_0x50 = glGetUniformLocation(this->field_0x4, "u7");
    this->field_0x54 = glGetUniformLocation(this->field_0x4, "u8");
    this->field_0x58 = glGetUniformLocation(this->field_0x4, "u9");

    glUseProgram(this->field_0x4);
    for (int i = 0; i != 2; i++) {
        int loc = (&this->field_0x44)[i];   // field_0x44 (u4), field_0x48 (u5)
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

} // namespace AbyssEngine

namespace AbyssEngine {

int BumpShaderV4::ShaderIndex;

__attribute__((minsize)) BumpShaderV4::BumpShaderV4()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    this->field_0x0 = (void *)(_ZTVN11AbyssEngine12BumpShaderV4E + 8);
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->field_0xc.s = u"BumpShaderV4";
}

} // namespace AbyssEngine
