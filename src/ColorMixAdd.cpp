#include "gof2/ColorMixAdd.h"




// ---- _ColorMixAdd_92388.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

void _ZN11AbyssEngine11ColorMixAddD0Ev(
    AbyssEngine::ColorMixAdd *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- UpdateMeshData_922a2.cpp ----
namespace AbyssEngine {

void ColorMixAdd::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_i32(this, 0x28), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x34) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x1c4);
    if (field_i32(this, 0x3c) >= 0)
        glUniform1i(field_i32(this, 0x3c), field_u8(mesh, 0x85));

    if (field_u8(this, 0x9) != 0) {
        glUniform4fv(field_i32(this, 0x2c), 1, (float *)((char *)engine + 0xd0));
        if (field_i32(this, 0x38) >= 0)
            glUniform1f(field_i32(this, 0x38), 1.0f - field_f32(mesh, 0x1c));
        field_u8(this, 0x9) = 0;
    }

    if ((field_u8(mesh, 0x0) & 2) != 0) {
        glEnableVertexAttribArray(field_i32(this, 0x20));
        glEnableVertexAttribArray(field_i32(this, 0x24));
        if (field_u8(mesh, 0x5c) == 0) {
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        } else {
            glBindBuffer(0x8892, field_i32(mesh, 0x60));
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, field_i32(mesh, 0x68));
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        }
    }
}

} // namespace AbyssEngine

// ---- SetInActive_92284.cpp ----
namespace AbyssEngine {

void ColorMixAdd::SetInActive()
{
    for (int i = 2; i != 0; i = i - 1) {
        glDisableVertexAttribArray(field_i32(this, 0x20));
        glDisableVertexAttribArray(field_i32(this, 0x24));
    }
}

} // namespace AbyssEngine

// ---- ColorMixAdd_9213c.cpp ----
extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(
    AbyssEngine::ShaderBaseStruct *self);

namespace AbyssEngine {

int ColorMixAdd::ShaderIndex;

ColorMixAdd::ColorMixAdd()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);
    *(void **)this = (void *)(_ZTVN11AbyssEngine11ColorMixAddE + 8);
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    String tmp;
    tmp.s = u"ColorMixAdd";
    this->name = tmp;
}

} // namespace AbyssEngine

// ---- Init_921c8.cpp ----
namespace AbyssEngine {

void ColorMixAdd::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("ColorMixAdd.vsh", "ColorMixAdd.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x30) = glGetUniformLocation(program, "u0");
    field_i32(this, 0x20) = glGetAttribLocation(field_i32(this, 0x04), "a0");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a1");
    field_i32(this, 0x28) = glGetUniformLocation(field_i32(this, 0x04), "u1");
    field_i32(this, 0x2c) = glGetUniformLocation(field_i32(this, 0x04), "u2");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u3");
    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u5");

    glUseProgram(field_i32(this, 0x04));
    glUniform1i(field_i32(this, 0x30), 0);
}

} // namespace AbyssEngine
