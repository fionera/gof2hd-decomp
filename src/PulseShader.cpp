#include "gof2/PulseShader.h"
#include "gof2/ShaderBaseStruct.h"
#include "gof2/String.h"
#include "gof2/Mesh.h"
#include "gof2/ApplicationManager.h"

// The decompiler dropped the receiver from the original
// ApplicationManager_GetCurrentTimeMillis() call; it reads from the global
// ApplicationManager singleton. Recover it here as the receiver.
extern ApplicationManager *g_ApplicationManager;

namespace AbyssEngine {

// ---- SetInActive_92554.cpp ----
void PulseShader::SetInActive()
{
    if (this->attrA0 >= 0) glDisableVertexAttribArray(this->attrA0);
    if (this->attrA1 >= 0) glDisableVertexAttribArray(this->attrA1);
    if (this->attrA2 >= 0) glDisableVertexAttribArray(this->attrA2);
    if (this->attrA3 >= 0) glDisableVertexAttribArray(this->attrA3);
    if (this->attrA4 >= 0) glDisableVertexAttribArray(this->attrA4);
}

// ---- Init_92424.cpp ----
void PulseShader::Init(::Engine *)
{
    int program = ((ShaderBaseStruct *)(this))->ES2LoadProgram("PulseShader.vsh", "PulseShader.fsh");
    this->program = program;

    this->attrA0 = glGetAttribLocation(program, "a0");
    this->attrA1 = glGetAttribLocation(this->program, "a1");
    this->attrA2 = glGetAttribLocation(this->program, "a2");
    this->attrA3 = glGetAttribLocation(this->program, "a3");
    this->attrA4 = glGetAttribLocation(this->program, "a4");

    this->uniU0 = glGetUniformLocation(this->program, "u0");
    this->uniU1 = glGetUniformLocation(this->program, "u1");
    this->uniU2 = glGetUniformLocation(this->program, "u2");
    this->uniU3 = glGetUniformLocation(this->program, "u3");
    this->uniU4 = glGetUniformLocation(this->program, "u4");
    this->uniU5 = glGetUniformLocation(this->program, "u5");
    this->uniU6 = glGetUniformLocation(this->program, "u6");
    this->uniU7 = glGetUniformLocation(this->program, "u7");
    this->uniU8 = glGetUniformLocation(this->program, "u8");
    this->uniU9 = glGetUniformLocation(this->program, "u9");

    glUseProgram(this->program);
    glUniform1i(this->uniU5, 0);
}

} // namespace AbyssEngine

// ---- _PulseShader_927d0.cpp ----
void _ZN11AbyssEngine11PulseShaderD0Ev(PulseShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    ::operator delete(base);
}

// ---- UpdateMeshData_92594.cpp ----
extern "C" {
float __aeabi_l2f(long long v);
float sinf(float x);
extern float PulseShader_timeScale;
}

namespace AbyssEngine {

void PulseShader::UpdateMeshData(Mesh *mesh, ::Engine *engine)
{
    if (this->uniU0 >= 0)
        glUniformMatrix4fv(this->uniU0, 1, 0, (float *)((char *)engine + 0x104));
    if (this->uniU1 >= 0)
        glUniformMatrix3fv(this->uniU1, 1, 0, (float *)((char *)engine + 0x204));

    if (this->uniformsDirty != 0) {
        if (this->uniU2 >= 0)
            glUniform3f(this->uniU2, f32(engine, 0x330), f32(engine, 0x334),
                        f32(engine, 0x338));
        if (this->uniU3 >= 0)
            glUniform3f(this->uniU3, f32(engine, 0x34c), f32(engine, 0x350),
                        f32(engine, 0x354));
        if (this->uniU6 >= 0)
            glUniform4fv(this->uniU6, 1, (float *)((char *)engine + 0xd0));
        if (this->uniU7 >= 0)
            glUniform4fv(this->uniU7, 1, (float *)((char *)engine + 0x2a8));
        if (this->uniU8 >= 0)
            glUniform4fv(this->uniU8, 1, (float *)((char *)engine + 0x298));
        if (this->uniU9 >= 0)
            glUniform4fv(this->uniU9, 1, (float *)((char *)engine + 0x2b8));

        long long t = g_ApplicationManager->GetCurrentTimeMillis();
        float v = sinf(__aeabi_l2f(t) / PulseShader_timeScale);
        glUniform1f(this->uniU4, v + 2.0f);
        this->uniformsDirty = 0;
    }

    void (*enableVertexAttribArray)(int) = glEnableVertexAttribArray;
    if (this->attrA0 >= 0) enableVertexAttribArray(this->attrA0);
    if (this->attrA1 >= 0) enableVertexAttribArray(this->attrA1);
    if (this->attrA2 >= 0) enableVertexAttribArray(this->attrA2);
    if (this->attrA3 >= 0) enableVertexAttribArray(this->attrA3);
    if (this->attrA4 >= 0) enableVertexAttribArray(this->attrA4);

    if (mesh->field_0x5c == 0) {
        if (this->attrA0 >= 0)
            glVertexAttribPointer(this->attrA0, 3, 0x1406, 0, 0, mesh->field_0x4);
        if (this->attrA1 >= 0)
            glVertexAttribPointer(this->attrA1, 2, 0x1406, 0, 0, mesh->field_0x8);
        if (this->attrA2 >= 0)
            glVertexAttribPointer(this->attrA2, 3, 0x1406, 0, 0, mesh->field_0x10);
        if (this->attrA3 >= 0)
            glVertexAttribPointer(this->attrA3, 3, 0x1406, 0, 0, (const void *)(uintptr_t)mesh->field_0x14);
        if (this->attrA4 >= 0)
            glVertexAttribPointer(this->attrA4, 3, 0x1406, 0, 0, (const void *)(uintptr_t)mesh->field_0x18);
    } else {
        void (*bindBuffer)(unsigned int, unsigned int) = glBindBuffer;
        void (*vertexAttribPointer)(int, int, unsigned int, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, mesh->field_0x60);
        vertexAttribPointer(this->attrA0, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, mesh->field_0x68);
        vertexAttribPointer(this->attrA1, 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, mesh->field_0x6c);
        vertexAttribPointer(this->attrA2, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, mesh->field_0x70);
        vertexAttribPointer(this->attrA3, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, mesh->field_0x74);
        vertexAttribPointer(this->attrA4, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- PulseShader_92398.cpp ----
namespace AbyssEngine {

PulseShader::PulseShader()
{
    new ((AbyssEngine::ShaderBaseStruct *)this) ShaderBaseStruct();
    this->field_0x0 = (char *)PulseShader_vtable + 8;
    PulseShader_typeInfoDest = PulseShader_typeInfoSource;

    // name = String("PulseShader", false); this->field_0xc = name; ~name.
    String name;
    ((String *)(&name))->ctor_char(PulseShader_name, false);
    ((String *)(&this->field_0xc))->assign(&name);
    ((String *)(&name))->dtor();
}

} // namespace AbyssEngine
