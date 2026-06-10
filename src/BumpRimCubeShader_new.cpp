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
    loc = this->field_0x20;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x24;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x28;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x2c;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x30;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

// ---- Init_888ac.cpp ----
namespace AbyssEngine {

void BumpRimCubeShader_new::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->LoadBindShader("BumpRimCubeShader_new.vsh", "BumpRimCubeShader_new.fsh");
    this->field_0x4 = program;
    if (program == 0) {
        program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpRimCubeShader_new.vsh", "BumpRimCubeShader_new.fsh");
        this->field_0x4 = program;
    }

    this->field_0x20 = glGetAttribLocation(program, "a0");
    this->field_0x24 = glGetAttribLocation(this->field_0x4, "a1");
    this->field_0x28 = glGetAttribLocation(this->field_0x4, "a2");
    this->field_0x2c = glGetAttribLocation(this->field_0x4, "a3");
    this->field_0x30 = glGetAttribLocation(this->field_0x4, "a4");

    this->field_0x34 = glGetUniformLocation(this->field_0x4, "u0");
    this->field_0x38 = glGetUniformLocation(this->field_0x4, "u1");
    this->field_0x3c = glGetUniformLocation(this->field_0x4, "u2");
    this->field_0x40 = glGetUniformLocation(this->field_0x4, "u3");
    this->field_0x44 = glGetUniformLocation(this->field_0x4, "u4");
    this->field_0x48 = glGetUniformLocation(this->field_0x4, "u5");
    this->field_0x4c = glGetUniformLocation(this->field_0x4, "u6");
    this->field_0x50 = glGetUniformLocation(this->field_0x4, "u7");
    this->field_0x54 = glGetUniformLocation(this->field_0x4, "u8");
    this->field_0x58 = glGetUniformLocation(this->field_0x4, "u9");
    this->field_0x5c = glGetUniformLocation(this->field_0x4, "u10");
    this->field_0x60 = glGetUniformLocation(this->field_0x4, "u11");
    this->field_0x68 = glGetUniformLocation(this->field_0x4, "u12");
    this->field_0x64 = glGetUniformLocation(this->field_0x4, "u13");
    this->field_0x6c = glGetUniformLocation(this->field_0x4, "u14");
    this->field_0x70 = glGetUniformLocation(this->field_0x4, "u15");
    this->field_0x74 = glGetUniformLocation(this->field_0x4, "u16");
    this->field_0x78 = glGetUniformLocation(this->field_0x4, "u17");
    this->field_0x7c = glGetUniformLocation(this->field_0x4, "u18");
    this->field_0x80 = glGetUniformLocation(this->field_0x4, "u19");
    this->field_0x84 = glGetUniformLocation(this->field_0x4, "u20");
    this->field_0x88 = glGetUniformLocation(this->field_0x4, "u21");
    this->field_0x8c = glGetUniformLocation(this->field_0x4, "u22");
    this->field_0x90 = glGetUniformLocation(this->field_0x4, "u23");
    this->field_0x94 = glGetUniformLocation(this->field_0x4, "u24");

    glUseProgram(this->field_0x4);
    for (int i = 0; i != 2; i++) {
        int loc = (&this->field_0x4c)[i];
        if (loc >= 0)
            glUniform1i(loc, i);
    }
    glUniform1i(this->field_0x54, 7);
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
    if (this->field_0x34 >= 0)
        glUniformMatrix4fv(this->field_0x34, 1, 0, (char *)engine + 0x104);
    if (this->field_0x38 >= 0)
        glUniformMatrix3fv(this->field_0x38, 1, 0, (char *)engine + 0x204);
    if (this->field_0x3c >= 0)
        glUniformMatrix4fv(this->field_0x3c, 1, 0, (char *)engine + 0x144);
    if (this->field_0x78 >= 0)
        glUniform1f(this->field_0x78, g_rimnGlobalA);
    if (this->field_0x7c >= 0)
        glUniform1f(this->field_0x7c, g_rimnGlobalB);
    if (this->field_0x94 >= 0)
        glUniform1f(this->field_0x94, g_rimnGlobalC);

    if (this->field_0x9 != 0) {
        glUniform3f(this->field_0x40, field_f32(engine, 0x330),
                    field_f32(engine, 0x334), field_f32(engine, 0x338));
        if (this->field_0x48 >= 0)
            glUniform3f(this->field_0x48, field_f32(engine, 0x34c),
                        field_f32(engine, 0x350), field_f32(engine, 0x354));
        if (this->field_0x58 >= 0)
            glUniform4fv(this->field_0x58, 1, (float *)((char *)engine + 0xd0));
        float buf[3];
        if (this->field_0x5c >= 0) {
            buf[0] = field_f32(engine, 0x2cc) + field_f32(engine, 0x2d8);
            buf[1] = field_f32(engine, 0x2d0) + field_f32(engine, 0x2dc);
            buf[2] = field_f32(engine, 0x2d4) + field_f32(engine, 0x2e0);
            glUniform3fv(this->field_0x5c, 1, buf);
        }
        if (this->field_0x60 >= 0)
            glUniform3fv(this->field_0x60, 1, (float *)((char *)engine + 0x2fc));
        if (this->field_0x68 >= 0)
            glUniform3fv(this->field_0x68, 1, (float *)((char *)engine + 0x2e4));
        if (this->field_0x70 >= 0)
            glUniform1f(this->field_0x70, field_f32(engine, 0x2c8));
        if (this->field_0x74 >= 0)
            glUniform3fv(this->field_0x74, 1, (float *)((char *)engine + 800));
        int loc84 = this->field_0x84;
        if (loc84 >= 0) {
            float *v = _ZN11AbyssEngine6AEMath6VectorcvPfEv((char *)engine + 0x3f0);
            glUniform3fv(loc84, 1, v);
        }
        if (this->field_0x8c >= 0)
            glUniform1f(this->field_0x8c, field_f32(engine, 1000));
        if (this->field_0x88 >= 0)
            glUniform1f(this->field_0x88, field_f32(engine, 0x3ec));
        if (this->field_0x90 >= 0)
            glUniform1i(this->field_0x90, g_rimnByteGlobal);
        if (this->field_0x80 >= 0) {
            float v = 0.0f;
            int *m30 = (int *)field_ptr(mesh, 0x30);
            if (m30 != 0) {
                v = 1.0f;
                if (m30[9] == 0)
                    v = 0.0f;
            }
            glUniform1f(this->field_0x80, v);
        }
        int loc5c = this->field_0x5c;
        if (field_i32(engine, 0x32c) >= 2) {
            if (loc5c >= 0) {
                buf[0] = field_f32(engine, 0x2cc) + field_f32(engine, 0x2d8);
                buf[1] = field_f32(engine, 0x2d0) + field_f32(engine, 0x2dc);
                buf[2] = field_f32(engine, 0x2d4) + field_f32(engine, 0x2e0);
                glUniform3fv(loc5c, 1, buf);
            }
            glUniform3fv(this->field_0x64, 1, (float *)((char *)engine + 0x308));
            glUniform3fv(this->field_0x6c, 1, (float *)((char *)engine + 0x2f0));
            glUniform3f(this->field_0x44, field_f32(engine, 0x33c),
                        field_f32(engine, 0x340), field_f32(engine, 0x344));
        } else {
            glUniform3f(loc5c, 0, 0, 0);
            glUniform3f(this->field_0x64, 0, 0, 0);
            glUniform3f(this->field_0x6c, 0, 0, 0);
            glUniform3f(this->field_0x44, field_f32(engine, 0x33c),
                        field_f32(engine, 0x340), field_f32(engine, 0x344));
        }
        this->field_0x9 = 0;
    }

    if (this->field_0x20 >= 0)
        glEnableVertexAttribArray(this->field_0x20);
    if (this->field_0x24 >= 0)
        glEnableVertexAttribArray(this->field_0x24);
    if (this->field_0x28 >= 0)
        glEnableVertexAttribArray(this->field_0x28);
    if (this->field_0x2c >= 0)
        glEnableVertexAttribArray(this->field_0x2c);
    if (this->field_0x30 >= 0)
        glEnableVertexAttribArray(this->field_0x30);

    if (field_u8(mesh, 0x5c) == 0) {
        if (this->field_0x20 >= 0)
            glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (this->field_0x24 >= 0)
            glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (this->field_0x28 >= 0)
            glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        if (this->field_0x2c >= 0)
            glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        if (this->field_0x30 >= 0)
            glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x70));
        glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x74));
        glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, 0);
    }
    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine

// ---- BumpRimCubeShader_new_88820.cpp ----
extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(
    AbyssEngine::ShaderBaseStruct *self);
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringC1EPKcb(
    AbyssEngine::String *self, const char *text, bool copy);
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringaSERKS0_(
    AbyssEngine::String *self, const AbyssEngine::String *other);
extern "C" void _ZN11AbyssEngine6StringD1Ev(AbyssEngine::String *self);

namespace AbyssEngine {

int BumpRimCubeShader_new::ShaderIndex;

struct BumpRimConstructorFrame {
    char name[sizeof(String)];
    volatile uint32_t stackGuard;
};

__attribute__((minsize)) BumpRimCubeShader_new::BumpRimCubeShader_new()
{
    BumpRimConstructorFrame frame;
    frame.stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;

    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);
    this->field_0x0 = _ZTVN11AbyssEngine21BumpRimCubeShader_newE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;

    _ZN11AbyssEngine6StringC1EPKcb((String *)frame.name, "BumpRimCubeShader_new", false);
    _ZN11AbyssEngine6StringaSERKS0_(&this->field_0xc, (String *)frame.name);
    _ZN11AbyssEngine6StringD1Ev((String *)frame.name);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - frame.stackGuard;
    if (guardDelta != 0)
        __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
