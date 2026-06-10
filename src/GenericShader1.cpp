#include "gof2/GenericShader1.h"


extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(ShaderBaseStruct *self);

// ---- UpdateMeshData_8ed8a.cpp ----
namespace AbyssEngine {

void GenericShader1::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x34) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x38) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x38), 1, 0, (char *)engine + 0x204);

    if (field_u8(this, 0x9) != 0) {
        if (field_i32(this, 0x3c) >= 0)
            glUniform3f(field_i32(this, 0x3c), field_f32(engine, 0x330), field_f32(engine, 0x334),
                        field_f32(engine, 0x338));
        if (field_i32(this, 0x40) >= 0)
            glUniform3f(field_i32(this, 0x40), field_f32(engine, 0x34c), field_f32(engine, 0x350),
                        field_f32(engine, 0x354));
        if (field_i32(this, 0x48) >= 0)
            glUniform4fv(field_i32(this, 0x48), 1, (float *)((char *)engine + 0xd0));
        if (field_i32(this, 0x4c) >= 0)
            glUniform4fv(field_i32(this, 0x4c), 1, (float *)((char *)engine + 0x2a8));
        if (field_i32(this, 0x50) >= 0)
            glUniform4fv(field_i32(this, 0x50), 1, (float *)((char *)engine + 0x298));
        if (field_i32(this, 0x54) >= 0)
            glUniform4fv(field_i32(this, 0x54), 1, (float *)((char *)engine + 0x2b8));
        field_u8(this, 0x9) = 0;
    }

    if (field_i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x20));
    if (field_i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x24));
    if (field_i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x28));
    if (field_i32(this, 0x2c) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x2c));
    if (field_i32(this, 0x30) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x30));

    if (field_i32(this, 0x20) >= 0)
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    if (field_i32(this, 0x24) >= 0)
        glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
    if (field_i32(this, 0x28) >= 0)
        glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
    if (field_i32(this, 0x2c) >= 0)
        glVertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
    if (field_i32(this, 0x30) >= 0)
        glVertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
}

} // namespace AbyssEngine

// ---- SetInActive_8ed4c.cpp ----
namespace AbyssEngine {

void GenericShader1::SetInActive()
{
    int loc;
    loc = field_i32(this, 0x20);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x24);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x28);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x2c);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x30);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

// ---- Init_8ec2c.cpp ----
namespace AbyssEngine {

void GenericShader1::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("GenericShader1.vsh", "GenericShader1.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_normal");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a_tangent");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a_binormal");
    field_i32(this, 0x30) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");

    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u_m0");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u_m1");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u_m2");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u_m3");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u_m4");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u_m5");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u_m6");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u_m7");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u_m8");

    glUseProgram(field_i32(this, 0x04));
    return glUniform1i(field_i32(this, 0x44), 0);
}

} // namespace AbyssEngine

// ---- _GenericShader1_8eee2.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine14GenericShader1D0Ev(
    AbyssEngine::GenericShader1 *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- GenericShader1_8eba0.cpp ----
namespace AbyssEngine {

// AbyssEngine::GenericShader1::GenericShader1()
// Constructs the ShaderBaseStruct base, installs the vtable, publishes the instance
// to a global pointer slot, and assigns the shader name string to field 0xc.
extern int *g_genericShader1GlobalSlot; // *(DAT_0009ec24 + 0x9ebc6)
extern int *g_genericShader1GlobalSrc;  // *(DAT_0009ec1c + 0x9ebbe)

GenericShader1::GenericShader1()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);

    // Install vtable: *this = &vtable + 8 (skip the two RTTI/offset words).
    *(void **)this = (void *)(_ZTVN11AbyssEngine14GenericShader1E + 8);

    // Publish *src into the global pointer slot.
    *g_genericShader1GlobalSlot = *g_genericShader1GlobalSrc;

    String tmp("GenericShader1", false);
    this->field_0xc = tmp;
    tmp.~String();
}

} // namespace AbyssEngine
