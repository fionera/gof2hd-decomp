#include "gof2/NoTexShader.h"

namespace AbyssEngine {

// AbyssEngine::NoTexShader::SetInActive()
void NoTexShader::SetInActive()
{
    return glDisableVertexAttribArray(field_0x20);
}

// AbyssEngine::NoTexShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void NoTexShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    glUniformMatrix4fv(field_0x24, 1, 0, e + 0x104);
    if (field_0x9 != 0) {
        glUniform4fv(field_0x28, 1, (const float *)(e + 0xd0));
        field_0x9 = 0;
    }
    glEnableVertexAttribArray(field_0x20);

    int size;
    const void *ptr;
    if (mesh == 0) {
        ptr = *(void **)(e + 0x348);
        size = 2;
    } else {
        if (*(uint8_t *)(m + 0x5c) == 0) {
            ptr = *(void **)(m + 0x4);
        } else {
            glBindBuffer(0x8892, *(uint32_t *)(m + 0x60));
            ptr = 0;
        }
        size = 3;
    }
    return glVertexAttribPointer(field_0x20, size, 0x1406, 0, 0, ptr);
}

// AbyssEngine::NoTexShader::Init(AbyssEngine::Engine*)
void NoTexShader::Init(Engine *)
{
    uint32_t program = ((ShaderBaseStruct *)((ShaderBaseStruct *)this))->ES2LoadProgram("NoTexShader.vsh", "NoTexShader.fsh");
    field_0x4 = (int)program;
    field_0x20 = glGetAttribLocation(program, "a_position");
    field_0x24 = glGetUniformLocation(field_0x4, "u_mvp");
    field_0x28 = glGetUniformLocation(field_0x4, "u_color");
    return glUseProgram(field_0x4);
}

// AbyssEngine::NoTexShader::NoTexShader()
NoTexShader::NoTexShader()
{
    ShaderBaseStruct_ctor((ShaderBaseStruct *)this);
    field_0x0 = (char *)NoTexShader_vtable + 8;
    NoTexShader_ShaderIndex = ShaderBaseStruct_shaderIndexIntern;
    field_0xc.s = u"NoTexShader";
    return;
}

} // namespace AbyssEngine

// AbyssEngine::NoTexShader::~NoTexShader() (deleting dtor)
void _ZN11AbyssEngine11NoTexShaderD0Ev(AbyssEngine::NoTexShader *self)
{
    ::operator delete(ShaderBaseStruct_dtor((AbyssEngine::ShaderBaseStruct *)self));
}
