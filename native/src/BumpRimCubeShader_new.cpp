#include "gof2/BumpRimCubeShader_new.h"


extern "C" void glUniform3fv(int location, int count, const float *value);
extern "C" float *_ZN11AbyssEngine6AEMath6VectorcvPfEv(void *self);
extern "C" float g_rimnGlobalA;
extern "C" float g_rimnGlobalB;
extern "C" float g_rimnGlobalC;
extern "C" unsigned char g_rimnByteGlobal;

// ---- SetInActive_88b18.cpp ----
namespace AbyssEngine {

void BumpRimCubeShader_new::SetInActive()
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

// ---- Init_888ac.cpp ----
namespace AbyssEngine {

void BumpRimCubeShader_new::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->LoadBindShader("BumpRimCubeShader_new.vsh", "BumpRimCubeShader_new.fsh");
    field_i32(this, 0x04) = program;
    if (program == 0) {
        program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpRimCubeShader_new.vsh", "BumpRimCubeShader_new.fsh");
        field_i32(this, 0x04) = program;
    }

    field_i32(this, 0x20) = glGetAttribLocation(program, "a0");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a1");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a2");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a3");
    field_i32(this, 0x30) = glGetAttribLocation(field_i32(this, 0x04), "a4");

    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u0");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u1");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u2");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u3");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u5");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u6");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u7");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u8");
    field_i32(this, 0x58) = glGetUniformLocation(field_i32(this, 0x04), "u9");
    field_i32(this, 0x5c) = glGetUniformLocation(field_i32(this, 0x04), "u10");
    field_i32(this, 0x60) = glGetUniformLocation(field_i32(this, 0x04), "u11");
    field_i32(this, 0x68) = glGetUniformLocation(field_i32(this, 0x04), "u12");
    field_i32(this, 0x64) = glGetUniformLocation(field_i32(this, 0x04), "u13");
    field_i32(this, 0x6c) = glGetUniformLocation(field_i32(this, 0x04), "u14");
    field_i32(this, 0x70) = glGetUniformLocation(field_i32(this, 0x04), "u15");
    field_i32(this, 0x74) = glGetUniformLocation(field_i32(this, 0x04), "u16");
    field_i32(this, 0x78) = glGetUniformLocation(field_i32(this, 0x04), "u17");
    field_i32(this, 0x7c) = glGetUniformLocation(field_i32(this, 0x04), "u18");
    field_i32(this, 0x80) = glGetUniformLocation(field_i32(this, 0x04), "u19");
    field_i32(this, 0x84) = glGetUniformLocation(field_i32(this, 0x04), "u20");
    field_i32(this, 0x88) = glGetUniformLocation(field_i32(this, 0x04), "u21");
    field_i32(this, 0x8c) = glGetUniformLocation(field_i32(this, 0x04), "u22");
    field_i32(this, 0x90) = glGetUniformLocation(field_i32(this, 0x04), "u23");
    field_i32(this, 0x94) = glGetUniformLocation(field_i32(this, 0x04), "u24");

    glUseProgram(field_i32(this, 0x04));
    for (int i = 0; i != 2; i++) {
        int loc = field_i32(this, i * 4 + 0x4c);
        if (loc >= 0)
            glUniform1i(loc, i);
    }
    glUniform1i(field_i32(this, 0x54), 7);
}

} // namespace AbyssEngine

// ---- _BumpRimCubeShader_new_88f40.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine21BumpRimCubeShader_newD0Ev(
    AbyssEngine::BumpRimCubeShader_new *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- UpdateMeshData_88b58.cpp ----
namespace AbyssEngine {

void BumpRimCubeShader_new::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    void *volatile cookie = __stack_chk_guard;
    if (field_i32(this, 0x34) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x38) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x38), 1, 0, (char *)engine + 0x204);
    if (field_i32(this, 0x3c) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x3c), 1, 0, (char *)engine + 0x144);
    if (field_i32(this, 0x78) >= 0)
        glUniform1f(field_i32(this, 0x78), g_rimnGlobalA);
    if (field_i32(this, 0x7c) >= 0)
        glUniform1f(field_i32(this, 0x7c), g_rimnGlobalB);
    if (field_i32(this, 0x94) >= 0)
        glUniform1f(field_i32(this, 0x94), g_rimnGlobalC);

    if (field_u8(this, 0x9) != 0) {
        glUniform3f(field_i32(this, 0x40), field_f32(engine, 0x330),
                    field_f32(engine, 0x334), field_f32(engine, 0x338));
        if (field_i32(this, 0x48) >= 0)
            glUniform3f(field_i32(this, 0x48), field_f32(engine, 0x34c),
                        field_f32(engine, 0x350), field_f32(engine, 0x354));
        if (field_i32(this, 0x58) >= 0)
            glUniform4fv(field_i32(this, 0x58), 1, (float *)((char *)engine + 0xd0));
        float buf[3];
        if (field_i32(this, 0x5c) >= 0) {
            buf[0] = field_f32(engine, 0x2cc) + field_f32(engine, 0x2d8);
            buf[1] = field_f32(engine, 0x2d0) + field_f32(engine, 0x2dc);
            buf[2] = field_f32(engine, 0x2d4) + field_f32(engine, 0x2e0);
            glUniform3fv(field_i32(this, 0x5c), 1, buf);
        }
        if (field_i32(this, 0x60) >= 0)
            glUniform3fv(field_i32(this, 0x60), 1, (float *)((char *)engine + 0x2fc));
        if (field_i32(this, 0x68) >= 0)
            glUniform3fv(field_i32(this, 0x68), 1, (float *)((char *)engine + 0x2e4));
        if (field_i32(this, 0x70) >= 0)
            glUniform1f(field_i32(this, 0x70), field_f32(engine, 0x2c8));
        if (field_i32(this, 0x74) >= 0)
            glUniform3fv(field_i32(this, 0x74), 1, (float *)((char *)engine + 800));
        int loc84 = field_i32(this, 0x84);
        if (loc84 >= 0) {
            float *v = _ZN11AbyssEngine6AEMath6VectorcvPfEv((char *)engine + 0x3f0);
            glUniform3fv(loc84, 1, v);
        }
        if (field_i32(this, 0x8c) >= 0)
            glUniform1f(field_i32(this, 0x8c), field_f32(engine, 1000));
        if (field_i32(this, 0x88) >= 0)
            glUniform1f(field_i32(this, 0x88), field_f32(engine, 0x3ec));
        if (field_i32(this, 0x90) >= 0)
            glUniform1i(field_i32(this, 0x90), g_rimnByteGlobal);
        if (field_i32(this, 0x80) >= 0) {
            float v = 0.0f;
            int *m30 = (int *)field_ptr(mesh, 0x30);
            if (m30 != 0) {
                v = 1.0f;
                if (m30[9] == 0)
                    v = 0.0f;
            }
            glUniform1f(field_i32(this, 0x80), v);
        }
        int loc5c = field_i32(this, 0x5c);
        if (field_i32(engine, 0x32c) >= 2) {
            if (loc5c >= 0) {
                buf[0] = field_f32(engine, 0x2cc) + field_f32(engine, 0x2d8);
                buf[1] = field_f32(engine, 0x2d0) + field_f32(engine, 0x2dc);
                buf[2] = field_f32(engine, 0x2d4) + field_f32(engine, 0x2e0);
                glUniform3fv(loc5c, 1, buf);
            }
            glUniform3fv(field_i32(this, 0x64), 1, (float *)((char *)engine + 0x308));
            glUniform3fv(field_i32(this, 0x6c), 1, (float *)((char *)engine + 0x2f0));
            glUniform3f(field_i32(this, 0x44), field_f32(engine, 0x33c),
                        field_f32(engine, 0x340), field_f32(engine, 0x344));
        } else {
            glUniform3f(loc5c, 0, 0, 0);
            glUniform3f(field_i32(this, 0x64), 0, 0, 0);
            glUniform3f(field_i32(this, 0x6c), 0, 0, 0);
            glUniform3f(field_i32(this, 0x44), field_f32(engine, 0x33c),
                        field_f32(engine, 0x340), field_f32(engine, 0x344));
        }
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

    if (field_u8(mesh, 0x5c) == 0) {
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
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x70));
        glVertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x74));
        glVertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, 0);
    }
    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine

// ---- BumpRimCubeShader_new_88820.cpp ----
extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(
    AbyssEngine::ShaderBaseStruct *self);

namespace AbyssEngine {

int BumpRimCubeShader_new::ShaderIndex;

__attribute__((minsize)) BumpRimCubeShader_new::BumpRimCubeShader_new()
{
    void *volatile cookie = __stack_chk_guard;
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);
    *(uint32_t *)this = (uint32_t)_ZTVN11AbyssEngine21BumpRimCubeShader_newE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    {
        String tmp("BumpRimCubeShader_new", false);
        this->field_0xc = tmp;
    }
    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
