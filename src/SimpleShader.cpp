#include "gof2/SimpleShader.h"

namespace AbyssEngine {

// AbyssEngine::SimpleShader::SetInActive()
void SimpleShader::SetInActive()
{
    return glDisableVertexAttribArray(field_0x20);
}

// AbyssEngine::SimpleShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void SimpleShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;
    glUniformMatrix4fv(field_0x24, 1, 0, e + 0x104);
    if (field_0x9 != 0) {
        glUniform4fv(field_0x28, 1, (const float *)(e + 0xd0));
        field_0x9 = 0;
    }
    glEnableVertexAttribArray(field_0x20);
    if (*(uint8_t *)(m + 0x5c) != 0) {
        glBindBuffer(0x8892, *(uint32_t *)(m + 0x60));
        return glVertexAttribPointer(field_0x20, 3, 0x1406, 0, 0, 0);
    }
    return glVertexAttribPointer(field_0x20, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
}

// AbyssEngine::SimpleShader::Init(AbyssEngine::Engine*)
void SimpleShader::Init(Engine *)
{
    uint32_t program = ShaderBaseStruct_ES2LoadProgram((ShaderBaseStruct *)this, "SimpleShader.vsh", "SimpleShader.fsh");
    field_0x4 = (int)program;
    field_0x20 = glGetAttribLocation(program, "a_position");
    field_0x24 = glGetUniformLocation(field_0x4, "u_WorldMatrix");
    field_0x28 = glGetUniformLocation(field_0x4, "u_color");
    return glUseProgram(field_0x4);
}

// AbyssEngine::SimpleShader::SimpleShader()
__attribute__((minsize)) SimpleShader::SimpleShader()
{
    ShaderBaseStruct_ctor((ShaderBaseStruct *)this);
    field_0xc.s = u"SimpleShader";
    field_0x0 = (char *)SimpleShader_vtable + 8;
    SimpleShader_ShaderIndex = ShaderBaseStruct_shaderIndexIntern;
    return;
}

} // namespace AbyssEngine

// ---- _SimpleShader_890b0.cpp ----
// AbyssEngine::SimpleShader::~SimpleShader() (deleting dtor)
void _ZN11AbyssEngine12SimpleShaderD0Ev(SimpleShader *self)
{
    operator_delete(ShaderBaseStruct_dtor((ShaderBaseStruct *)self));
}
