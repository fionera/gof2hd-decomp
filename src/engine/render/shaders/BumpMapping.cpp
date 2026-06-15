#include "gof2/engine/render/shaders/BumpMapping.h"
#include "gof2/platform/gl.h"

namespace AbyssEngine {

int BumpMapping::ShaderIndex;

BumpMapping::BumpMapping()
{
    this->vtable = _ZTVN11AbyssEngine11BumpMappingE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"BumpMapping";
}

void BumpMapping::Init(Engine *)
{
    this->program = this->ES2LoadProgram("BumpMapping.vsh", "BumpMapping.fsh");

    this->a0Loc = glGetAttribLocation(this->program, "a_position");
    this->a1Loc = glGetAttribLocation(this->program, "a_normal");
    this->a2Loc = glGetAttribLocation(this->program, "a_tangent");
    this->a3Loc = glGetAttribLocation(this->program, "a_binormal");
    this->a4Loc = glGetAttribLocation(this->program, "a_texCoord");

    this->u0Loc = glGetUniformLocation(this->program, "u_mvpMatrix");
    this->u1Loc = glGetUniformLocation(this->program, "u_lightDir");
    this->u2Loc = glGetUniformLocation(this->program, "u_texture");
    this->u3Loc = glGetUniformLocation(this->program, "u_normalMap");

    glUseProgram(this->program);
    glUniform1i(this->u2Loc, 0);
    glUniform1i(this->u3Loc, 1);
}

void BumpMapping::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    this->dirty = 0;

    glUniformMatrix4fv(this->u0Loc, 1, 0, (const float *)((char *)engine + 0x104));
    glUniform3f(this->u1Loc, field_f32(engine, 0x330), field_f32(engine, 0x334),
                field_f32(engine, 0x338));

    glEnableVertexAttribArray(this->a0Loc);
    glEnableVertexAttribArray(this->a1Loc);
    glEnableVertexAttribArray(this->a2Loc);
    glEnableVertexAttribArray(this->a3Loc);
    glEnableVertexAttribArray(this->a4Loc);

    glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    if ((field_u8(mesh, 0x0) & 2) != 0) {
        glVertexAttribPointer(this->a4Loc, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
    }
    if ((field_u8(mesh, 0x0) & 4) != 0) {
        glVertexAttribPointer(this->a1Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        glVertexAttribPointer(this->a2Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        glVertexAttribPointer(this->a3Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    }
}

void BumpMapping::SetInActive()
{
    glDisableVertexAttribArray(this->a0Loc);
    glDisableVertexAttribArray(this->a1Loc);
    glDisableVertexAttribArray(this->a2Loc);
    glDisableVertexAttribArray(this->a3Loc);
    glDisableVertexAttribArray(this->a4Loc);
}

} // namespace AbyssEngine
