#include "gof2/engine/render/shaders/ColorMixAdd.h"
#include "gof2/platform/gl.h"

namespace AbyssEngine {

int ColorMixAdd::ShaderIndex;

ColorMixAdd::ColorMixAdd()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    this->vtable = (void *)(_ZTVN11AbyssEngine11ColorMixAddE + 8);
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"ColorMixAdd";
}

void ColorMixAdd::Init(Engine *)
{
    int program = this->ES2LoadProgram("ColorMixAdd.vsh", "ColorMixAdd.fsh");
    this->program = program;

    this->u0Loc = glGetUniformLocation(program, "u0");
    this->a0Loc = glGetAttribLocation(this->program, "a0");
    this->a1Loc = glGetAttribLocation(this->program, "a1");
    this->u1Loc = glGetUniformLocation(this->program, "u1");
    this->u2Loc = glGetUniformLocation(this->program, "u2");
    this->u3Loc = glGetUniformLocation(this->program, "u3");
    this->u4Loc = glGetUniformLocation(this->program, "u4");
    this->u5Loc = glGetUniformLocation(this->program, "u5");

    glUseProgram(this->program);
    glUniform1i(this->u0Loc, 0);
}

void ColorMixAdd::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(this->u1Loc, 1, 0, (float *)((char *)engine + 0x104));
    if (this->u4Loc >= 0)
        glUniformMatrix4fv(this->u4Loc, 1, 0, (float *)((char *)engine + 0x1c4));
    if (this->u5Loc >= 0)
        glUniform1i(this->u5Loc, field_u8(mesh, 0x85));

    if (this->dirty != 0) {
        glUniform4fv(this->u2Loc, 1, (float *)((char *)engine + 0xd0));
        if (this->u3Loc >= 0)
            glUniform1f(this->u3Loc, 1.0f - field_f32(mesh, 0x1c));
        this->dirty = 0;
    }

    if ((field_u8(mesh, 0x0) & 2) != 0) {
        glEnableVertexAttribArray(this->a0Loc);
        glEnableVertexAttribArray(this->a1Loc);
        if (field_u8(mesh, 0x5c) == 0) {
            glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
            glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        } else {
            glBindBuffer(0x8892, field_i32(mesh, 0x60));
            glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, field_i32(mesh, 0x68));
            glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, 0);
        }
    }
}

void ColorMixAdd::SetInActive()
{
    for (int i = 2; i != 0; i = i - 1) {
        glDisableVertexAttribArray(this->a0Loc);
        glDisableVertexAttribArray(this->a1Loc);
    }
}

} // namespace AbyssEngine
