#include "gof2/MaskShader.h"
#include "gof2/Engine.h"




// ---- _MaskShader_9177c.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine10MaskShaderD0Ev(
    AbyssEngine::MaskShader *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- SetInActive_91630.cpp ----
namespace AbyssEngine {

void MaskShader::SetInActive()
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

// ---- UpdateMeshData_9165a.cpp ----
extern "C" void _ZN11AbyssEngine6Engine14SetTextureSlotEjj(
    AbyssEngine::Engine *self, uint32_t a, uint32_t b);

namespace AbyssEngine {

void MaskShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x2c) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x2c), 1, 0, (char *)engine + 0x104);

    if (field_u8(this, 0x9) != 0) {
        if (field_i32(this, 0x38) >= 0)
            glUniform4fv(field_i32(this, 0x38), 1, (float *)((char *)engine + 0xd0));
        field_u8(this, 0x9) = 0;
    }

    if (field_i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x20));
    if (field_i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x24));
    if (field_i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x28));

    char *texBase = *(char **)(engine->field_0x30);
    int *tex = *(int **)(texBase + 0x20);
    if (tex != 0)
        _ZN11AbyssEngine6Engine14SetTextureSlotEjj(engine, tex[1], 1);

    if (field_u8(mesh, 0x5c) == 0) {
        if (field_i32(this, 0x20) >= 0)
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (field_i32(this, 0x24) >= 0)
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (tex == 0)
            return;
        if (field_i32(this, 0x28) < 0)
            return;
        glVertexAttribPointer(field_i32(this, 0x28), 2, 0x1406, 0, 0, *(void **)(*tex + 8));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        if (tex == 0)
            return;
        glBindBuffer(0x8892, *(int *)(*tex + 0x68));
        glVertexAttribPointer(field_i32(this, 0x28), 2, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- Init_9156c.cpp ----
namespace AbyssEngine {

void MaskShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("MaskShader.vsh", "MaskShader.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a_color");

    field_i32(this, 0x2c) = glGetUniformLocation(field_i32(this, 0x04), "u_mvpMatrix");
    field_i32(this, 0x30) = glGetUniformLocation(field_i32(this, 0x04), "u_texture0");
    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u_texture1");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u_color");

    glUseProgram(field_i32(this, 0x04));
    for (int i = 0; i != 2; i++) {
        int loc = field_i32(this, i * 4 + 0x30);
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

} // namespace AbyssEngine

// ---- MaskShader_914e0.cpp ----
extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(
    AbyssEngine::ShaderBaseStruct *self);

namespace AbyssEngine {

int MaskShader::ShaderIndex;

// AbyssEngine::MaskShader::MaskShader()
MaskShader::MaskShader()
{
    void *volatile cookie = __stack_chk_guard;

    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);
    *(uint32_t *)this = (uint32_t)_ZTVN11AbyssEngine10MaskShaderE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    {
        String tmp("MaskShader", false);
        this->field_0xc = tmp;
    }

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
