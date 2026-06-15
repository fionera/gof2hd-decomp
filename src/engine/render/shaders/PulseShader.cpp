#include "gof2/engine/render/shaders/PulseShader.h"
#include "gof2/game/core/String.h"
#include "gof2/engine/render/Mesh.h"
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/platform/gl.h"
#include "gof2/platform/libc.h"

// PulseShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" void *_ZTVN11AbyssEngine11PulseShaderE[];

// The decompiler dropped the receiver from the original
// ApplicationManager_GetCurrentTimeMillis() call; it reads from the global
// ApplicationManager singleton. Recover it here as the receiver.
extern ApplicationManager *g_ApplicationManager;

extern "C" {
float sinf(float x);
extern float PulseShader_timeScale;
}

namespace AbyssEngine {

PulseShader::PulseShader()
{
    this->vtable = (char *)_ZTVN11AbyssEngine11PulseShaderE + 8;
    this->name.s = u"PulseShader";
}

void PulseShader::SetInActive()
{
    if (this->a0Loc >= 0) glDisableVertexAttribArray(this->a0Loc);
    if (this->a1Loc >= 0) glDisableVertexAttribArray(this->a1Loc);
    if (this->a2Loc >= 0) glDisableVertexAttribArray(this->a2Loc);
    if (this->a3Loc >= 0) glDisableVertexAttribArray(this->a3Loc);
    if (this->a4Loc >= 0) glDisableVertexAttribArray(this->a4Loc);
}

void PulseShader::Init(Engine *)
{
    int program = this->ES2LoadProgram("PulseShader.vsh", "PulseShader.fsh");
    this->program = program;

    this->a0Loc = glGetAttribLocation(program, "a0");
    this->a1Loc = glGetAttribLocation(this->program, "a1");
    this->a2Loc = glGetAttribLocation(this->program, "a2");
    this->a3Loc = glGetAttribLocation(this->program, "a3");
    this->a4Loc = glGetAttribLocation(this->program, "a4");

    this->u0Loc = glGetUniformLocation(this->program, "u0");
    this->u1Loc = glGetUniformLocation(this->program, "u1");
    this->u2Loc = glGetUniformLocation(this->program, "u2");
    this->u3Loc = glGetUniformLocation(this->program, "u3");
    this->u4Loc = glGetUniformLocation(this->program, "u4");
    this->u5Loc = glGetUniformLocation(this->program, "u5");
    this->u6Loc = glGetUniformLocation(this->program, "u6");
    this->u7Loc = glGetUniformLocation(this->program, "u7");
    this->u8Loc = glGetUniformLocation(this->program, "u8");
    this->u9Loc = glGetUniformLocation(this->program, "u9");

    glUseProgram(this->program);
    glUniform1i(this->u5Loc, 0);
}

void PulseShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->u0Loc >= 0)
        glUniformMatrix4fv(this->u0Loc, 1, 0, (float *)((char *)engine + 0x104));
    if (this->u1Loc >= 0)
        glUniformMatrix3fv(this->u1Loc, 1, 0, (float *)((char *)engine + 0x204));

    if (this->dirty != 0) {
        if (this->u2Loc >= 0)
            glUniform3f(this->u2Loc, f32(engine, 0x330), f32(engine, 0x334),
                        f32(engine, 0x338));
        if (this->u3Loc >= 0)
            glUniform3f(this->u3Loc, f32(engine, 0x34c), f32(engine, 0x350),
                        f32(engine, 0x354));
        if (this->u6Loc >= 0)
            glUniform4fv(this->u6Loc, 1, (float *)((char *)engine + 0xd0));
        if (this->u7Loc >= 0)
            glUniform4fv(this->u7Loc, 1, (float *)((char *)engine + 0x2a8));
        if (this->u8Loc >= 0)
            glUniform4fv(this->u8Loc, 1, (float *)((char *)engine + 0x298));
        if (this->u9Loc >= 0)
            glUniform4fv(this->u9Loc, 1, (float *)((char *)engine + 0x2b8));

        long long t = g_ApplicationManager->GetCurrentTimeMillis();
        float v = sinf(__aeabi_l2f(t) / PulseShader_timeScale);
        glUniform1f(this->u4Loc, v + 2.0f);
        this->dirty = 0;
    }

    if (this->a0Loc >= 0) glEnableVertexAttribArray(this->a0Loc);
    if (this->a1Loc >= 0) glEnableVertexAttribArray(this->a1Loc);
    if (this->a2Loc >= 0) glEnableVertexAttribArray(this->a2Loc);
    if (this->a3Loc >= 0) glEnableVertexAttribArray(this->a3Loc);
    if (this->a4Loc >= 0) glEnableVertexAttribArray(this->a4Loc);

    if (mesh->field_0x5c == 0) {
        if (this->a0Loc >= 0)
            glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, mesh->field_0x4);
        if (this->a1Loc >= 0)
            glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, mesh->field_0x8);
        if (this->a2Loc >= 0)
            glVertexAttribPointer(this->a2Loc, 3, 0x1406, 0, 0, mesh->field_0x10);
        if (this->a3Loc >= 0)
            glVertexAttribPointer(this->a3Loc, 3, 0x1406, 0, 0, (const void *)(uintptr_t)mesh->field_0x14);
        if (this->a4Loc >= 0)
            glVertexAttribPointer(this->a4Loc, 3, 0x1406, 0, 0, (const void *)(uintptr_t)mesh->field_0x18);
    } else {
        glBindBuffer(0x8892, mesh->field_0x60);
        glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x68);
        glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x6c);
        glVertexAttribPointer(this->a2Loc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x70);
        glVertexAttribPointer(this->a3Loc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x74);
        glVertexAttribPointer(this->a4Loc, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
