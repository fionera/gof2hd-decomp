#include "SimpleShader.h"




// ---- SetInActive_89038.cpp ----
namespace AbyssEngine {

void SimpleShader::SetInActive()
{
    return glDisableVertexAttribArray(field_u32(this, 0x20));
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8903e.cpp ----
namespace AbyssEngine {

void SimpleShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_i32(this, 0x24), 1, 0, bytes(engine) + 0x104);
    if (field_u8(this, 0x9) != 0) {
        glUniform4fv(field_i32(this, 0x28), 1, (const float *)(bytes(engine) + 0xd0));
        field_u8(this, 0x9) = 0;
    }
    glEnableVertexAttribArray(field_u32(this, 0x20));
    if (field_u8(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, field_u32(mesh, 0x60));
        return glVertexAttribPointer(field_u32(this, 0x20), 3, 0x1406, 0, 0, 0);
    }
    return glVertexAttribPointer(field_u32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
}

} // namespace AbyssEngine

// ---- _SimpleShader_890b0.cpp ----
extern "C" void _ZN11AbyssEngine12SimpleShaderD0Ev(SimpleShader *self)
{
    operator delete(ShaderBaseStruct_dtor((ShaderBaseStruct *)self));
}

// ---- Init_88fdc.cpp ----
namespace AbyssEngine {

void SimpleShader::Init(Engine *)
{
    uint32_t program = ShaderBaseStruct_ES2LoadProgram((ShaderBaseStruct *)this, "SimpleShader.vsh", "SimpleShader.fsh");
    field_u32(this, 0x4) = program;
    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetUniformLocation(field_u32(this, 0x4), "u_WorldMatrix");
    field_i32(this, 0x28) = glGetUniformLocation(field_u32(this, 0x4), "u_color");
    return glUseProgram(field_u32(this, 0x4));
}

} // namespace AbyssEngine

// ---- SimpleShader_88f50.cpp ----
namespace AbyssEngine {

__attribute__((minsize)) SimpleShader::SimpleShader()
{
    void *volatile cookie = __stack_chk_guard;
    ShaderBaseStruct_ctor((ShaderBaseStruct *)this);
    *(String *)((char *)this + 0xc) = String("SimpleShader", false);
    *(void **)this = (char *)SimpleShader_vtable + 8;
    SimpleShader_ShaderIndex = ShaderBaseStruct_shaderIndexIntern;
    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
