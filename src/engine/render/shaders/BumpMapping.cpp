#include "gof2/engine/render/shaders/BumpMapping.h"

void _ZN11AbyssEngine11BumpMappingD0Ev(
    AbyssEngine::BumpMapping *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

namespace AbyssEngine {

void BumpMapping::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpMapping.vsh", "BumpMapping.fsh");
    this->field_0x4 = program;

    this->a0Loc = glGetAttribLocation(program, "a_position");
    this->a1Loc = glGetAttribLocation(this->field_0x4, "a_normal");
    this->a2Loc = glGetAttribLocation(this->field_0x4, "a_tangent");
    this->a3Loc = glGetAttribLocation(this->field_0x4, "a_binormal");
    this->a4Loc = glGetAttribLocation(this->field_0x4, "a_texCoord");

    this->u0Loc = glGetUniformLocation(this->field_0x4, "u_mvpMatrix");
    this->u1Loc = glGetUniformLocation(this->field_0x4, "u_lightDir");
    this->u2Loc = glGetUniformLocation(this->field_0x4, "u_texture");
    this->u3Loc = glGetUniformLocation(this->field_0x4, "u_normalMap");

    glUseProgram(this->field_0x4);
    glUniform1i(this->u2Loc, 0);
    return glUniform1i(this->u3Loc, 1);
}

} // namespace AbyssEngine

namespace AbyssEngine {

void BumpMapping::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->dirty != 0)
        this->dirty = 0;

    glUniformMatrix4fv(this->u0Loc, 1, 0, (char *)engine + 0x104);
    glUniform3f(this->u1Loc, field_f32(engine, 0x330), field_f32(engine, 0x334),
                field_f32(engine, 0x338));

    void (*enableVertexAttribArray)(uint32_t) = glEnableVertexAttribArray;
    enableVertexAttribArray(this->a0Loc);
    enableVertexAttribArray(this->a1Loc);
    enableVertexAttribArray(this->a2Loc);
    enableVertexAttribArray(this->a3Loc);
    enableVertexAttribArray(this->a4Loc);

    glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    if ((field_u8(mesh, 0x0) & 2) != 0) {
        glVertexAttribPointer(this->a4Loc, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
    }
    if ((field_u8(mesh, 0x0) & 4) != 0) {
        void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
            glVertexAttribPointer;
        vertexAttribPointer(this->a1Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        vertexAttribPointer(this->a2Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        vertexAttribPointer(this->a3Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    }
}

} // namespace AbyssEngine

namespace AbyssEngine {

int BumpMapping::ShaderIndex;

BumpMapping::BumpMapping()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    this->field_0x0 = _ZTVN11AbyssEngine11BumpMappingE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"BumpMapping";
}

} // namespace AbyssEngine

namespace AbyssEngine {

void BumpMapping::SetInActive()
{
    void (*disableVertexAttribArray)(uint32_t) = glDisableVertexAttribArray;

    disableVertexAttribArray(this->a0Loc);
    disableVertexAttribArray(this->a1Loc);
    disableVertexAttribArray(this->a2Loc);
    disableVertexAttribArray(this->a3Loc);
    disableVertexAttribArray(this->a4Loc);
}

} // namespace AbyssEngine
