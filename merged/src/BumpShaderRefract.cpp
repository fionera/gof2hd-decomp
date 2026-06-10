#include "BumpShaderRefract.h"


extern "C" void glActiveTexture(uint32_t texture);
extern "C" void _ZN11AbyssEngine6Engine16ActivateRefractFBOEv(AbyssEngine::Engine *self);
extern "C" void glUniform2f(int location, float x, float y);
extern "C" int _ZN11AbyssEngine6Engine14GetDisplayWidthEv(AbyssEngine::Engine *self);
extern "C" int _ZN11AbyssEngine6Engine15GetDisplayHeightEv(AbyssEngine::Engine *self);

// ---- SetInActive_8f910.cpp ----
namespace AbyssEngine {

void BumpShaderRefract::SetInActive()
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
}

} // namespace AbyssEngine

// ---- _BumpShaderRefract_8fac2.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine17BumpShaderRefractD0Ev(
    AbyssEngine::BumpShaderRefract *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- BumpShaderRefract_8f760.cpp ----
extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(
    AbyssEngine::ShaderBaseStruct *self);

namespace AbyssEngine {

int BumpShaderRefract::ShaderIndex;

__attribute__((minsize)) BumpShaderRefract::BumpShaderRefract()
{
    void *volatile cookie = __stack_chk_guard;
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);
    *(uint32_t *)this = (uint32_t)_ZTVN11AbyssEngine12BumpShaderRefractE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    {
        String tmp("BumpShaderRefract", false);
        *(String *)((char *)this + 0xc) = tmp;
    }
    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine

// ---- Init_8f7ec.cpp ----
namespace AbyssEngine {

void BumpShaderRefract::Init(Engine *engine)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpShaderRefract.vsh", "BumpShaderRefract.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a0");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a1");

    field_i32(this, 0x28) = glGetUniformLocation(field_i32(this, 0x04), "u0");
    field_i32(this, 0x2c) = glGetUniformLocation(field_i32(this, 0x04), "u1");
    field_i32(this, 0x30) = glGetUniformLocation(field_i32(this, 0x04), "u2");
    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u3");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u5");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u6");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u7");

    glActiveTexture(0x84c7);
    _ZN11AbyssEngine6Engine16ActivateRefractFBOEv(engine);
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u8");

    glUseProgram(field_i32(this, 0x04));
    for (int i = 0; i != 2; i++) {
        int loc = field_i32(this, i * 4 + 0x30);
        if (loc >= 0)
            glUniform1i(loc, i);
    }
    glUniform1i(field_i32(this, 0x3c), 7);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8f93a.cpp ----
namespace AbyssEngine {

void BumpShaderRefract::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x2c) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x2c), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x40) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x40), 1, 0, (char *)engine + 0x1c4);

    if (field_u8(this, 0x9) != 0) {
        if (field_i32(this, 0x44) >= 0)
            glUniform1i(field_i32(this, 0x44), field_u8(mesh, 0x85));
        glUniform4fv(field_i32(this, 0x48), 1, (float *)((char *)engine + 0xd0));
        int loc = field_i32(this, 0x38);
        if (loc >= 0) {
            float w = (float)_ZN11AbyssEngine6Engine14GetDisplayWidthEv(engine);
            float h = (float)_ZN11AbyssEngine6Engine15GetDisplayHeightEv(engine);
            glUniform2f(loc, 1.0f / w, 1.0f / h);
        }
        glActiveTexture(0x84c7);
        _ZN11AbyssEngine6Engine16ActivateRefractFBOEv(engine);
        field_u8(this, 0x9) = 0;
    }

    if (field_i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x20));
    if (field_i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x24));
    if (field_i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x28));

    int loc0 = field_i32(this, 0x20);
    const void *ptr;
    int last;
    if (field_u8(mesh, 0x5c) == 0) {
        if (loc0 >= 0)
            glVertexAttribPointer(loc0, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (field_i32(this, 0x24) >= 0)
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        last = field_i32(this, 0x28);
        if (last < 0)
            return;
        ptr = field_ptr(mesh, 0xc);
    } else {
        if (loc0 >= 0) {
            glBindBuffer(0x8892, field_i32(mesh, 0x60));
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        }
        if (field_i32(this, 0x24) >= 0) {
            glBindBuffer(0x8892, field_i32(mesh, 0x68));
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        }
        if (field_i32(this, 0x28) < 0)
            return;
        glBindBuffer(0x8892, field_i32(mesh, 0x78));
        ptr = 0;
        last = field_i32(this, 0x28);
    }
    glVertexAttribPointer(last, 4, 0x1406, 0, 0, ptr);
}

} // namespace AbyssEngine
