#include "gof2/engine/render/shaders/GreenShader.h"

namespace AbyssEngine {

void GreenShader::SetInActive()
{
    if (this->a0Loc >= 0)
        glDisableVertexAttribArray(this->a0Loc);
    if (this->a1Loc >= 0)
        glDisableVertexAttribArray(this->a1Loc);
    if (this->a2Loc >= 0)
        glDisableVertexAttribArray(this->a2Loc);
    if (this->a3Loc >= 0)
        glDisableVertexAttribArray(this->a3Loc);
    if (this->a4Loc >= 0)
        glDisableVertexAttribArray(this->a4Loc);
}

} // namespace AbyssEngine

void _ZN11AbyssEngine11GreenShaderD0Ev(AbyssEngine::GreenShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    ::operator delete(base);
}

namespace AbyssEngine {

void GreenShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)(this))->ES2LoadProgram("GreenShader.vsh", "GreenShader.fsh");
    this->field_0x4 = program;

    this->a0Loc = glGetAttribLocation(program, "a0");
    this->a1Loc = glGetAttribLocation(this->field_0x4, "a1");
    this->a2Loc = glGetAttribLocation(this->field_0x4, "a2");
    this->a3Loc = glGetAttribLocation(this->field_0x4, "a3");
    this->a4Loc = glGetAttribLocation(this->field_0x4, "a4");

    this->u0Loc = glGetUniformLocation(this->field_0x4, "u0");
    this->u1Loc = glGetUniformLocation(this->field_0x4, "u1");
    this->u2Loc = glGetUniformLocation(this->field_0x4, "u2");
    this->u3Loc = glGetUniformLocation(this->field_0x4, "u3");
    this->u4Loc = glGetUniformLocation(this->field_0x4, "u4");
    this->u5Loc = glGetUniformLocation(this->field_0x4, "u5");
    this->u6Loc = glGetUniformLocation(this->field_0x4, "u6");
    this->u7Loc = glGetUniformLocation(this->field_0x4, "u7");
    this->u8Loc = glGetUniformLocation(this->field_0x4, "u8");

    glUseProgram(this->field_0x4);
    return glUniform1i(this->u4Loc, 0);
}

} // namespace AbyssEngine

namespace AbyssEngine {

void GreenShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->u0Loc >= 0)
        glUniformMatrix4fv(this->u0Loc, 1, 0, (const float *)((char *)engine + 0x104));

    if (this->a0Loc >= 0)
        glEnableVertexAttribArray(this->a0Loc);
    if (this->a1Loc >= 0)
        glEnableVertexAttribArray(this->a1Loc);
    if (this->a2Loc >= 0)
        glEnableVertexAttribArray(this->a2Loc);
    if (this->a3Loc >= 0)
        glEnableVertexAttribArray(this->a3Loc);
    if (this->a4Loc >= 0)
        glEnableVertexAttribArray(this->a4Loc);

    if (u8(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, i32(mesh, 0x60));
        glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, 0);
    } else {
        if (this->a0Loc < 0)
            return;
        glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, (const void *)i32(mesh, 4));
    }
}

} // namespace AbyssEngine

namespace AbyssEngine {

GreenShader::GreenShader()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    this->field_0x0 = (void *)(GreenShader_vtable + 8);
    this->name = GreenShader_name;
}

} // namespace AbyssEngine
