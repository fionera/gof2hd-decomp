#include "gof2/PulseShader.h"
#include "gof2/Mesh.h"

namespace AbyssEngine {

// ---- SetInActive_92554.cpp ----
void PulseShader::SetInActive()
{
    if (this->field_0x20 >= 0) glDisableVertexAttribArray(this->field_0x20);
    if (this->field_0x24 >= 0) glDisableVertexAttribArray(this->field_0x24);
    if (this->field_0x28 >= 0) glDisableVertexAttribArray(this->field_0x28);
    if (this->field_0x2c >= 0) glDisableVertexAttribArray(this->field_0x2c);
    if (this->field_0x30 >= 0) glDisableVertexAttribArray(this->field_0x30);
}

// ---- Init_92424.cpp ----
void PulseShader::Init(::Engine *)
{
    int program = ShaderBaseStruct_ES2LoadProgram(this, "PulseShader.vsh", "PulseShader.fsh");
    this->field_0x4 = program;

    this->field_0x20 = glGetAttribLocation(program, "a0");
    this->field_0x24 = glGetAttribLocation(this->field_0x4, "a1");
    this->field_0x28 = glGetAttribLocation(this->field_0x4, "a2");
    this->field_0x2c = glGetAttribLocation(this->field_0x4, "a3");
    this->field_0x30 = glGetAttribLocation(this->field_0x4, "a4");

    this->field_0x34 = glGetUniformLocation(this->field_0x4, "u0");
    this->field_0x38 = glGetUniformLocation(this->field_0x4, "u1");
    this->field_0x3c = glGetUniformLocation(this->field_0x4, "u2");
    this->field_0x40 = glGetUniformLocation(this->field_0x4, "u3");
    this->field_0x58 = glGetUniformLocation(this->field_0x4, "u4");
    this->field_0x44 = glGetUniformLocation(this->field_0x4, "u5");
    this->field_0x48 = glGetUniformLocation(this->field_0x4, "u6");
    this->field_0x4c = glGetUniformLocation(this->field_0x4, "u7");
    this->field_0x50 = glGetUniformLocation(this->field_0x4, "u8");
    this->field_0x54 = glGetUniformLocation(this->field_0x4, "u9");

    glUseProgram(this->field_0x4);
    glUniform1i(this->field_0x44, 0);
}

} // namespace AbyssEngine

// ---- _PulseShader_927d0.cpp ----
extern "C" void _ZN11AbyssEngine11PulseShaderD0Ev(PulseShader *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}

// ---- UpdateMeshData_92594.cpp ----
extern "C" {
long long ApplicationManager_GetCurrentTimeMillis();
float __aeabi_l2f(long long v);
float sinf(float x);
extern float PulseShader_timeScale;
}

namespace AbyssEngine {

void PulseShader::UpdateMeshData(Mesh *mesh, ::Engine *engine)
{
    if (this->field_0x34 >= 0)
        glUniformMatrix4fv(this->field_0x34, 1, 0, (float *)((char *)engine + 0x104));
    if (this->field_0x38 >= 0)
        glUniformMatrix3fv(this->field_0x38, 1, 0, (float *)((char *)engine + 0x204));

    if (this->field_0x9 != 0) {
        if (this->field_0x3c >= 0)
            glUniform3f(this->field_0x3c, f32(engine, 0x330), f32(engine, 0x334),
                        f32(engine, 0x338));
        if (this->field_0x40 >= 0)
            glUniform3f(this->field_0x40, f32(engine, 0x34c), f32(engine, 0x350),
                        f32(engine, 0x354));
        if (this->field_0x48 >= 0)
            glUniform4fv(this->field_0x48, 1, (float *)((char *)engine + 0xd0));
        if (this->field_0x4c >= 0)
            glUniform4fv(this->field_0x4c, 1, (float *)((char *)engine + 0x2a8));
        if (this->field_0x50 >= 0)
            glUniform4fv(this->field_0x50, 1, (float *)((char *)engine + 0x298));
        if (this->field_0x54 >= 0)
            glUniform4fv(this->field_0x54, 1, (float *)((char *)engine + 0x2b8));

        long long t = ApplicationManager_GetCurrentTimeMillis();
        float v = sinf(__aeabi_l2f(t) / PulseShader_timeScale);
        glUniform1f(this->field_0x58, v + 2.0f);
        this->field_0x9 = 0;
    }

    void (*enableVertexAttribArray)(int) = glEnableVertexAttribArray;
    if (this->field_0x20 >= 0) enableVertexAttribArray(this->field_0x20);
    if (this->field_0x24 >= 0) enableVertexAttribArray(this->field_0x24);
    if (this->field_0x28 >= 0) enableVertexAttribArray(this->field_0x28);
    if (this->field_0x2c >= 0) enableVertexAttribArray(this->field_0x2c);
    if (this->field_0x30 >= 0) enableVertexAttribArray(this->field_0x30);

    if (mesh->field_0x5c == 0) {
        if (this->field_0x20 >= 0)
            glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, mesh->field_0x4);
        if (this->field_0x24 >= 0)
            glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, mesh->field_0x8);
        if (this->field_0x28 >= 0)
            glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, mesh->field_0x10);
        if (this->field_0x2c >= 0)
            glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, (const void *)(uintptr_t)mesh->field_0x14);
        if (this->field_0x30 >= 0)
            glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, (const void *)(uintptr_t)mesh->field_0x18);
    } else {
        void (*bindBuffer)(unsigned int, unsigned int) = glBindBuffer;
        void (*vertexAttribPointer)(int, int, unsigned int, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, mesh->field_0x60);
        vertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, mesh->field_0x68);
        vertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, mesh->field_0x6c);
        vertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, mesh->field_0x70);
        vertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, mesh->field_0x74);
        vertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- PulseShader_92398.cpp ----
namespace AbyssEngine {

PulseShader::PulseShader()
{
    ShaderBaseStruct_ctor(this);
    this->field_0x0 = (char *)PulseShader_vtable + 8;
    PulseShader_typeInfoDest = PulseShader_typeInfoSource;

    // name = String("PulseShader", false); this->field_0xc = name; ~name.
    String name;
    String_ctor_char(&name, PulseShader_name, false);
    String_assign(&this->field_0xc, &name);
    String_dtor(&name);
}

} // namespace AbyssEngine
