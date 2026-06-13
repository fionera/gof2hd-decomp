#include "gof2/engine/render/shaders/GlowShader.h"

namespace AbyssEngine {

void GlowShader::SetInActive()
{
    if (this->a_positionLoc >= 0)
        glDisableVertexAttribArray(this->a_positionLoc);
    if (this->a_texCoordLoc < 0)
        return;
    return glDisableVertexAttribArray(this->a_texCoordLoc);
}

} // namespace AbyssEngine

void _ZN11AbyssEngine10GlowShaderD0Ev(
    AbyssEngine::GlowShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

namespace AbyssEngine {

void GlowShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->u_mvpLoc >= 0)
        glUniformMatrix4fv(this->u_mvpLoc, 1, 0, (char *)engine + 0x104);
    if (this->u_colorLoc >= 0)
        glUniformMatrix3fv(this->u_colorLoc, 1, 0, (char *)engine + 0x204);

    if (this->a_positionLoc >= 0)
        glEnableVertexAttribArray(this->a_positionLoc);
    if (this->a_texCoordLoc >= 0)
        glEnableVertexAttribArray(this->a_texCoordLoc);

    if (field_u8(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(this->a_positionLoc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        return glVertexAttribPointer(this->a_texCoordLoc, 2, 0x1406, 0, 0, 0);
    }
    if (this->a_positionLoc >= 0)
        glVertexAttribPointer(this->a_positionLoc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    int index = this->a_texCoordLoc;
    if (index < 0)
        return;
    return glVertexAttribPointer(index, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
}

} // namespace AbyssEngine

extern "C" AbyssEngine::GlowShader *
_ZN11AbyssEngine10GlowShaderC2Ev(AbyssEngine::GlowShader *self)
{
    new ((AbyssEngine::ShaderBaseStruct *)self) AbyssEngine::ShaderBaseStruct();
    self->field_0x0 = (void *)(_ZTVN11AbyssEngine10GlowShaderE + 8);
    AbyssEngine::GlowShader::ShaderIndex =
        AbyssEngine::ShaderBaseStruct::shaderIndexIntern;

    self->name = "GlowShader";
    return self;
}

namespace AbyssEngine {

void GlowShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("GlowShader.vsh", "GlowShader.fsh");
    this->field_0x4 = program;

    this->a_positionLoc = glGetAttribLocation(program, "a_position");
    this->a_texCoordLoc = glGetAttribLocation(this->field_0x4, "a_texCoord");

    this->u_mvpLoc = glGetUniformLocation(this->field_0x4, "u_mvp");
    this->u_colorLoc = glGetUniformLocation(this->field_0x4, "u_color");
    this->u_textureLoc = glGetUniformLocation(this->field_0x4, "u_texture");

    glUseProgram(this->field_0x4);
    return glUniform1i(this->u_textureLoc, 0);
}

} // namespace AbyssEngine
