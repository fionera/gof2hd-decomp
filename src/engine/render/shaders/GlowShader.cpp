#include "gof2/engine/render/shaders/GlowShader.h"
#include "gof2/platform/gl.h"

namespace AbyssEngine {

GlowShader::GlowShader()
{
    this->vtable = (void *)(_ZTVN11AbyssEngine10GlowShaderE + 8);
    GlowShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"GlowShader";
}

void GlowShader::Init(Engine *)
{
    this->program = this->ES2LoadProgram("GlowShader.vsh", "GlowShader.fsh");

    this->a_positionLoc = glGetAttribLocation(this->program, "a_position");
    this->a_texCoordLoc = glGetAttribLocation(this->program, "a_texCoord");

    this->u_mvpLoc = glGetUniformLocation(this->program, "u_mvp");
    this->u_colorLoc = glGetUniformLocation(this->program, "u_color");
    this->u_textureLoc = glGetUniformLocation(this->program, "u_texture");

    glUseProgram(this->program);
    glUniform1i(this->u_textureLoc, 0);
}

void GlowShader::SetInActive()
{
    if (this->a_positionLoc >= 0)
        glDisableVertexAttribArray(this->a_positionLoc);
    if (this->a_texCoordLoc >= 0)
        glDisableVertexAttribArray(this->a_texCoordLoc);
}

void GlowShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->u_mvpLoc >= 0)
        glUniformMatrix4fv(this->u_mvpLoc, 1, 0, (const float *)((char *)engine + 0x104));
    if (this->u_colorLoc >= 0)
        glUniformMatrix3fv(this->u_colorLoc, 1, 0, (const float *)((char *)engine + 0x204));

    if (this->a_positionLoc >= 0)
        glEnableVertexAttribArray(this->a_positionLoc);
    if (this->a_texCoordLoc >= 0)
        glEnableVertexAttribArray(this->a_texCoordLoc);

    if (field_u8(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(this->a_positionLoc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(this->a_texCoordLoc, 2, 0x1406, 0, 0, 0);
        return;
    }

    if (this->a_positionLoc >= 0)
        glVertexAttribPointer(this->a_positionLoc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    if (this->a_texCoordLoc >= 0)
        glVertexAttribPointer(this->a_texCoordLoc, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
}

} // namespace AbyssEngine
