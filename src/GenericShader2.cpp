#include "gof2/GenericShader2.h"

namespace AbyssEngine {

int GenericShader2::ShaderIndex;

// AbyssEngine::GenericShader2::SetInActive()
void GenericShader2::SetInActive()
{
    if (field_0x20 >= 0)
        glDisableVertexAttribArray(field_0x20);
    if (field_0x24 >= 0)
        glDisableVertexAttribArray(field_0x24);
    if (field_0x28 >= 0)
        glDisableVertexAttribArray(field_0x28);
    if (field_0x2c >= 0)
        glDisableVertexAttribArray(field_0x2c);
    if (field_0x30 >= 0)
        glDisableVertexAttribArray(field_0x30);
}

// AbyssEngine::GenericShader2::Init(AbyssEngine::Engine*)
void GenericShader2::Init(Engine *)
{
    uint32_t program = ShaderBaseStruct_ES2LoadProgram((ShaderBaseStruct *)this, "GenericShader2.vsh", "GenericShader2.fsh");
    field_0x4 = (int)program;

    field_0x20 = glGetAttribLocation(program, "a_position");
    field_0x24 = glGetAttribLocation(field_0x4, "a_normal");
    field_0x28 = glGetAttribLocation(field_0x4, "a_tangent");
    field_0x2c = glGetAttribLocation(field_0x4, "a_binormal");
    field_0x30 = glGetAttribLocation(field_0x4, "a_texCoord");

    field_0x34 = glGetUniformLocation(field_0x4, "u_m0");
    field_0x38 = glGetUniformLocation(field_0x4, "u_m1");
    field_0x3c = glGetUniformLocation(field_0x4, "u_m2");
    field_0x40 = glGetUniformLocation(field_0x4, "u_m3");
    field_0x44 = glGetUniformLocation(field_0x4, "u_m4");
    field_0x48 = glGetUniformLocation(field_0x4, "u_m5");
    field_0x4c = glGetUniformLocation(field_0x4, "u_m6");
    field_0x50 = glGetUniformLocation(field_0x4, "u_m7");
    field_0x54 = glGetUniformLocation(field_0x4, "u_m8");

    glUseProgram(field_0x4);
    glUniform1i(field_0x44, 0);
}

// AbyssEngine::GenericShader2::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void GenericShader2::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    if (field_0x34 >= 0)
        glUniformMatrix4fv(field_0x34, 1, 0, e + 0x104);
    if (field_0x38 >= 0)
        glUniformMatrix3fv(field_0x38, 1, 0, e + 0x204);

    if (field_0x9 != 0) {
        if (field_0x3c >= 0)
            glUniform3f(field_0x3c, *(float *)(e + 0x330), *(float *)(e + 0x334), *(float *)(e + 0x338));
        if (field_0x40 >= 0)
            glUniform3f(field_0x40, *(float *)(e + 0x34c), *(float *)(e + 0x350), *(float *)(e + 0x354));
        if (field_0x48 >= 0)
            glUniform4fv(field_0x48, 1, (const float *)(e + 0xd0));
        if (field_0x4c >= 0)
            glUniform4fv(field_0x4c, 1, (const float *)(e + 0x2a8));
        if (field_0x50 >= 0)
            glUniform4fv(field_0x50, 1, (const float *)(e + 0x298));
        if (field_0x54 >= 0)
            glUniform4fv(field_0x54, 1, (const float *)(e + 0x2b8));
        field_0x9 = 0;
    }

    if (field_0x20 >= 0)
        glEnableVertexAttribArray(field_0x20);
    if (field_0x24 >= 0)
        glEnableVertexAttribArray(field_0x24);
    if (field_0x28 >= 0)
        glEnableVertexAttribArray(field_0x28);
    if (field_0x2c >= 0)
        glEnableVertexAttribArray(field_0x2c);
    if (field_0x30 >= 0)
        glEnableVertexAttribArray(field_0x30);

    if (field_0x20 >= 0)
        glVertexAttribPointer(field_0x20, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
    if (field_0x24 >= 0)
        glVertexAttribPointer(field_0x24, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
    if (field_0x28 >= 0)
        glVertexAttribPointer(field_0x28, 3, 0x1406, 0, 0, *(void **)(m + 0x10));
    if (field_0x2c >= 0)
        glVertexAttribPointer(field_0x2c, 3, 0x1406, 0, 0, *(void **)(m + 0x14));
    if (field_0x30 >= 0)
        glVertexAttribPointer(field_0x30, 3, 0x1406, 0, 0, *(void **)(m + 0x18));
}

// AbyssEngine::GenericShader2::GenericShader2()
GenericShader2::GenericShader2()
{
    void *volatile cookie = __stack_chk_guard;
    ShaderBaseStruct_ctor((ShaderBaseStruct *)this);
    field_0x0 = (char *)GenericShader2_vtable + 8;
    ShaderIndex = ShaderBaseStruct_shaderIndexIntern;
    field_0xc.s = u"GenericShader2";
    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine

// ---- _GenericShader2_8e412.cpp ----
// AbyssEngine::GenericShader2::~GenericShader2() (deleting dtor)
void _ZN11AbyssEngine14GenericShader2D0Ev(AbyssEngine::GenericShader2 *self)
{
    operator_delete(ShaderBaseStruct_dtor((AbyssEngine::ShaderBaseStruct *)self));
}
