#include "gof2/NoTexVtxColorShader.h"

namespace AbyssEngine {

// AbyssEngine::NoTexVtxColorShader::SetInActive()
void NoTexVtxColorShader::SetInActive()
{
    glDisableVertexAttribArray(field_0x20);
    return glDisableVertexAttribArray(field_0x24);
}

// AbyssEngine::NoTexVtxColorShader::Init(AbyssEngine::Engine*)
void NoTexVtxColorShader::Init(Engine *)
{
    uint32_t program = ShaderBaseStruct_ES2LoadProgram((ShaderBaseStruct *)this, "NoTexVtxColorShader.vsh", "NoTexVtxColorShader.fsh");
    field_0x4 = (int)program;

    field_0x20 = glGetAttribLocation(program, "a_position");
    field_0x24 = glGetAttribLocation(field_0x4, "a_color");

    field_0x28 = glGetUniformLocation(field_0x4, "u_mvp");
    field_0x2c = glGetUniformLocation(field_0x4, "u_color");

    return glUseProgram(field_0x4);
}

// AbyssEngine::NoTexVtxColorShader::NoTexVtxColorShader()
NoTexVtxColorShader::NoTexVtxColorShader()
{
    void *volatile cookie = __stack_chk_guard;
    ShaderBaseStruct_ctor((ShaderBaseStruct *)this);
    field_0x0 = (char *)NoTexVtxColorShader_vtable + 8;
    NoTexVtxColorShader_ShaderIndex = ShaderBaseStruct_shaderIndexIntern;
    field_0xc.s = u"NoTexVtxColorShader";
    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

// AbyssEngine::NoTexVtxColorShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void NoTexVtxColorShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    glUniformMatrix4fv(field_0x28, 1, 0, e + 0x104);
    if (field_0x9 != 0) {
        glUniform4fv(field_0x2c, 1, (const float *)(e + 0xd0));
        field_0x9 = 0;
    }
    glEnableVertexAttribArray(field_0x20);
    glEnableVertexAttribArray(field_0x24);

    int index = field_0x20;
    int size;
    const void *ptr;
    int stride = 0;
    if (mesh == 0) {
        ptr = *(void **)(e + 0x348);
        size = 2;
    } else {
        glVertexAttribPointer(index, 3, 0x1406, 0, stride, *(void **)(m + 0x4));
        ptr = *(void **)(m + 0xc);
        index = field_0x24;
        size = 4;
    }
    return glVertexAttribPointer(index, size, 0x1406, 0, stride, ptr);
}

} // namespace AbyssEngine

// AbyssEngine::NoTexVtxColorShader::~NoTexVtxColorShader() (deleting dtor)
void _ZN11AbyssEngine19NoTexVtxColorShaderD0Ev(AbyssEngine::NoTexVtxColorShader *self)
{
    operator_delete(ShaderBaseStruct_dtor((AbyssEngine::ShaderBaseStruct *)self));
}
