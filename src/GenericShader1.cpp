#include "gof2/GenericShader1.h"
// NOTE: do not include gof2/ShaderBaseStruct.h here — GenericShader1.h already declares
// the minimal AbyssEngine::ShaderBaseStruct base layout this shader derives from, and the
// full class in ShaderBaseStruct.h would redefine it. The base's loader is reached via the
// ShaderBaseStruct_ES2LoadProgram free function declared in GenericShader1.h.

// ---- UpdateMeshData_8ed8a.cpp ----
namespace AbyssEngine {

// AbyssEngine::GenericShader1::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void GenericShader1::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    if (field_0x34 >= 0)
        glUniformMatrix4fv(field_0x34, 1, 0, e + 0x104);
    if (field_0x38 >= 0)
        glUniformMatrix3fv(field_0x38, 1, 0, e + 0x204);

    if (field_0x9 != 0) {
        if (field_0x3c >= 0)
            glUniform3f(field_0x3c, *(float *)(e + 0x330), *(float *)(e + 0x334),
                        *(float *)(e + 0x338));
        if (field_0x40 >= 0)
            glUniform3f(field_0x40, *(float *)(e + 0x34c), *(float *)(e + 0x350),
                        *(float *)(e + 0x354));
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

} // namespace AbyssEngine

// ---- SetInActive_8ed4c.cpp ----
namespace AbyssEngine {

// AbyssEngine::GenericShader1::SetInActive()
void GenericShader1::SetInActive()
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

} // namespace AbyssEngine

// ---- Init_8ec2c.cpp ----
namespace AbyssEngine {

// AbyssEngine::GenericShader1::Init(AbyssEngine::Engine*)
void GenericShader1::Init(Engine *)
{
    uint32_t program = ShaderBaseStruct_ES2LoadProgram(this, "GenericShader1.vsh", "GenericShader1.fsh");
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

} // namespace AbyssEngine

// ---- _GenericShader1_8eee2.cpp ----
// AbyssEngine::GenericShader1::~GenericShader1() (deleting dtor)
void _ZN11AbyssEngine14GenericShader1D0Ev(AbyssEngine::GenericShader1 *self)
{
    ::operator delete(ShaderBaseStruct_dtor((AbyssEngine::ShaderBaseStruct *)self));
}

// ---- GenericShader1_8eba0.cpp ----
namespace AbyssEngine {

// AbyssEngine::GenericShader1::GenericShader1()
// Constructs the ShaderBaseStruct base, installs the vtable, publishes the shader
// index into the engine's global slot, and assigns the shader name to field_0xc.
GenericShader1::GenericShader1()
{
    ShaderBaseStruct_ctor((ShaderBaseStruct *)this);
    field_0x0 = (char *)GenericShader1_vtable + 8;
    GenericShader1_ShaderIndex = ShaderBaseStruct_shaderIndexIntern;
    field_0xc.s = u"GenericShader1";
}

} // namespace AbyssEngine
