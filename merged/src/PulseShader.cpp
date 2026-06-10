#include "PulseShader.h"




// ---- SetInActive_92554.cpp ----
namespace AbyssEngine {

void PulseShader::SetInActive()
{
    if (i32(this, 0x20) >= 0)
        glDisableVertexAttribArray(i32(this, 0x20));
    if (i32(this, 0x24) >= 0)
        glDisableVertexAttribArray(i32(this, 0x24));
    if (i32(this, 0x28) >= 0)
        glDisableVertexAttribArray(i32(this, 0x28));
    if (i32(this, 0x2c) >= 0)
        glDisableVertexAttribArray(i32(this, 0x2c));
    if (i32(this, 0x30) >= 0)
        glDisableVertexAttribArray(i32(this, 0x30));
}

} // namespace AbyssEngine

// ---- Init_92424.cpp ----
namespace AbyssEngine {

void PulseShader::Init(Engine *)
{
    int program = ShaderBaseStruct_ES2LoadProgram(this, "PulseShader.vsh", "PulseShader.fsh");
    i32(this, 0x04) = program;

    i32(this, 0x20) = glGetAttribLocation(program, "a0");
    i32(this, 0x24) = glGetAttribLocation(i32(this, 0x04), "a1");
    i32(this, 0x28) = glGetAttribLocation(i32(this, 0x04), "a2");
    i32(this, 0x2c) = glGetAttribLocation(i32(this, 0x04), "a3");
    i32(this, 0x30) = glGetAttribLocation(i32(this, 0x04), "a4");

    i32(this, 0x34) = glGetUniformLocation(i32(this, 0x04), "u0");
    i32(this, 0x38) = glGetUniformLocation(i32(this, 0x04), "u1");
    i32(this, 0x3c) = glGetUniformLocation(i32(this, 0x04), "u2");
    i32(this, 0x40) = glGetUniformLocation(i32(this, 0x04), "u3");
    i32(this, 0x58) = glGetUniformLocation(i32(this, 0x04), "u4");
    i32(this, 0x44) = glGetUniformLocation(i32(this, 0x04), "u5");
    i32(this, 0x48) = glGetUniformLocation(i32(this, 0x04), "u6");
    i32(this, 0x4c) = glGetUniformLocation(i32(this, 0x04), "u7");
    i32(this, 0x50) = glGetUniformLocation(i32(this, 0x04), "u8");
    i32(this, 0x54) = glGetUniformLocation(i32(this, 0x04), "u9");

    glUseProgram(i32(this, 0x04));
    return glUniform1i(i32(this, 0x44), 0);
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

void PulseShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (i32(this, 0x34) >= 0)
        glUniformMatrix4fv(i32(this, 0x34), 1, 0, (float *)((char *)engine + 0x104));
    if (i32(this, 0x38) >= 0)
        glUniformMatrix3fv(i32(this, 0x38), 1, 0, (float *)((char *)engine + 0x204));

    if (u8(this, 0x9) != 0) {
        if (i32(this, 0x3c) >= 0)
            glUniform3f(i32(this, 0x3c), f32(engine, 0x330), f32(engine, 0x334),
                        f32(engine, 0x338));
        if (i32(this, 0x40) >= 0)
            glUniform3f(i32(this, 0x40), f32(engine, 0x34c), f32(engine, 0x350),
                        f32(engine, 0x354));
        if (i32(this, 0x48) >= 0)
            glUniform4fv(i32(this, 0x48), 1, (float *)((char *)engine + 0xd0));
        if (i32(this, 0x4c) >= 0)
            glUniform4fv(i32(this, 0x4c), 1, (float *)((char *)engine + 0x2a8));
        if (i32(this, 0x50) >= 0)
            glUniform4fv(i32(this, 0x50), 1, (float *)((char *)engine + 0x298));
        if (i32(this, 0x54) >= 0)
            glUniform4fv(i32(this, 0x54), 1, (float *)((char *)engine + 0x2b8));

        long long t = ApplicationManager_GetCurrentTimeMillis();
        float v = sinf(__aeabi_l2f(t) / PulseShader_timeScale);
        glUniform1f(i32(this, 0x58), v + 2.0f);
        u8(this, 0x9) = 0;
    }

    void (*enableVertexAttribArray)(int) = glEnableVertexAttribArray;
    if (i32(this, 0x20) >= 0)
        enableVertexAttribArray(i32(this, 0x20));
    if (i32(this, 0x24) >= 0)
        enableVertexAttribArray(i32(this, 0x24));
    if (i32(this, 0x28) >= 0)
        enableVertexAttribArray(i32(this, 0x28));
    if (i32(this, 0x2c) >= 0)
        enableVertexAttribArray(i32(this, 0x2c));
    if (i32(this, 0x30) >= 0)
        enableVertexAttribArray(i32(this, 0x30));

    if (u8(mesh, 0x5c) == 0) {
        if (i32(this, 0x20) >= 0)
            glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x4));
        if (i32(this, 0x24) >= 0)
            glVertexAttribPointer(i32(this, 0x24), 2, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x8));
        if (i32(this, 0x28) >= 0)
            glVertexAttribPointer(i32(this, 0x28), 3, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x10));
        if (i32(this, 0x2c) >= 0)
            glVertexAttribPointer(i32(this, 0x2c), 3, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x14));
        if (i32(this, 0x30) >= 0)
            glVertexAttribPointer(i32(this, 0x30), 3, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x18));
    } else {
        void (*bindBuffer)(unsigned int, unsigned int) = glBindBuffer;
        void (*vertexAttribPointer)(int, int, unsigned int, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, i32(mesh, 0x60));
        vertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(mesh, 0x68));
        vertexAttribPointer(i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(mesh, 0x6c));
        vertexAttribPointer(i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(mesh, 0x70));
        vertexAttribPointer(i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(mesh, 0x74));
        vertexAttribPointer(i32(this, 0x30), 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- PulseShader_92398.cpp ----
namespace AbyssEngine {

PulseShader::PulseShader()
{
    ShaderBaseStruct_ctor(this);
    *(int *)this = (int)(PulseShader_vtable) + 8;
    PulseShader_typeInfoDest = PulseShader_typeInfoSource;
    String tmp(PulseShader_name, false);
    *(String *)((char *)this + 0xc) = tmp;
}

} // namespace AbyssEngine
