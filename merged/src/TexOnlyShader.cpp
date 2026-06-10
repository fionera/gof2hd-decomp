#include "TexOnlyShader.h"




// ---- UpdateMeshData_8b74a.cpp ----
namespace AbyssEngine {

void TexOnlyShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field<int>(this, 0x28), 1, 0, (char *)engine + 0x104);
    if (field<uint8_t>(this, 0x9) != 0) {
        field<uint8_t>(this, 0x9) = 0;
    }
    glEnableVertexAttribArray(field<uint32_t>(this, 0x20));
    glEnableVertexAttribArray(field<uint32_t>(this, 0x24));

    if (field<uint8_t>(mesh, 0x5c) == 0) {
        glVertexAttribPointer(field<uint32_t>(this, 0x20), 3, 0x1406, 0, 0, field<void *>(mesh, 0x4));
        return glVertexAttribPointer(field<uint32_t>(this, 0x24), 2, 0x1406, 0, 0, field<void *>(mesh, 0x8));
    } else {
        glBindBuffer(0x8892, field<uint32_t>(mesh, 0x60));
        glVertexAttribPointer(field<uint32_t>(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field<uint32_t>(mesh, 0x68));
        return glVertexAttribPointer(field<uint32_t>(this, 0x24), 2, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- SetInActive_8b734.cpp ----
namespace AbyssEngine {

void TexOnlyShader::SetInActive()
{
    glDisableVertexAttribArray(field<uint32_t>(this, 0x20));
    return glDisableVertexAttribArray(field<uint32_t>(this, 0x24));
}

} // namespace AbyssEngine

// ---- Init_8b6c0.cpp ----
namespace AbyssEngine {

void TexOnlyShader::Init(Engine *)
{
    uint32_t program = ShaderBaseStruct_ES2LoadProgram((ShaderBaseStruct *)this, "TexOnlyShader.vsh",
                                                       "TexOnlyShader.fsh");
    field<uint32_t>(this, 0x4) = program;

    field<int>(this, 0x20) = glGetAttribLocation(program, "a_position");
    field<int>(this, 0x24) = glGetAttribLocation(field<uint32_t>(this, 0x4), "a_texCoord");
    field<int>(this, 0x28) = glGetUniformLocation(field<uint32_t>(this, 0x4), "u_WorldMatrix");
    field<int>(this, 0x2c) = glGetUniformLocation(field<uint32_t>(this, 0x4), "s_texture");

    glUseProgram(field<uint32_t>(this, 0x4));
    return glUniform1i(field<int>(this, 0x2c), 0);
}

} // namespace AbyssEngine

// ---- TexOnlyShader_8b634.cpp ----
namespace AbyssEngine {

TexOnlyShader::TexOnlyShader()
{
    void **guard = &__stack_chk_guard;
    void *volatile cookie = *guard;
    ShaderBaseStruct_ctor((ShaderBaseStruct *)this);
    field<uint32_t>(this, 0x0) = (uint32_t)g_TexOnlyShader_vtable + 8;
    g_TexOnlyShader_shaderIndex = g_ShaderBaseStruct_shaderIndexIntern;
    String tmp;
    String_ctor_char(&tmp, "TexOnlyShader", false);
    String_assign((String *)((char *)this + 0xc), &tmp);
    String_dtor(&tmp);

    uint32_t guardDelta = (uint32_t)*guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0) {
        __stack_chk_fail(guardDelta);
    }
}

} // namespace AbyssEngine

// ---- _TexOnlyShader_8b7e2.cpp ----
extern "C" void _ZN11AbyssEngine12TexOnlyShaderD0Ev(TexOnlyShader *self)
{
    operator delete(ShaderBaseStruct_dtor((ShaderBaseStruct *)self));
}
