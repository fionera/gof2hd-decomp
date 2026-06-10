#include "BumpShaderV2.h"


extern "C" int ShaderBaseStruct_shaderIndexIntern;
extern "C" int BumpShaderV2_ShaderIndex;

// ---- _BumpShaderV2_8dacc.cpp ----
extern "C" void _ZN11AbyssEngine12BumpShaderV2D0Ev(BumpShaderV2 *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}

// ---- SetInActive_8d888.cpp ----
namespace AbyssEngine {

void BumpShaderV2::SetInActive()
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

// ---- UpdateMeshData_8d8c8.cpp ----
namespace AbyssEngine {

// AbyssEngine::BumpShaderV2::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void BumpShaderV2::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *m = (char *)mesh;
    char *e = (char *)engine;

    if (i32(this, 0x38) >= 0)
        glUniformMatrix4fv(i32(this, 0x38), 1, 0, (const float *)(e + 0x104));
    if (i32(this, 0x3c) >= 0)
        glUniformMatrix3fv(i32(this, 0x3c), 1, 0, (const float *)(e + 0x204));

    // Per-material lighting block, uploaded once (gated by the +0x9 dirty flag).
    if (u8(this, 0x9)) {
        if (i32(this, 0x40) >= 0)
            glUniform3f(i32(this, 0x40), f32(e, 0x330), f32(e, 0x334), f32(e, 0x338));
        if (i32(this, 0x44) >= 0)
            glUniform3f(i32(this, 0x44), f32(e, 0x34c), f32(e, 0x350), f32(e, 0x354));
        if (i32(this, 0x50) >= 0)
            glUniform4fv(i32(this, 0x50), 1, (const float *)(e + 0xd0));
        if (i32(this, 0x54) >= 0)
            glUniform4fv(i32(this, 0x54), 1, (const float *)(e + 0x2a8));
        if (i32(this, 0x58) >= 0)
            glUniform4fv(i32(this, 0x58), 1, (const float *)(e + 0x298));
        if (i32(this, 0x5c) >= 0)
            glUniform4fv(i32(this, 0x5c), 1, (const float *)(e + 0x2b8));
        u8(this, 0x9) = 0;
    }

    if (i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(i32(this, 0x20));
    if (i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(i32(this, 0x24));
    if (i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(i32(this, 0x28));
    if (i32(this, 0x2c) >= 0)
        glEnableVertexAttribArray(i32(this, 0x2c));
    if (i32(this, 0x30) >= 0)
        glEnableVertexAttribArray(i32(this, 0x30));

    if (u8(m, 0x5c) == 0) {
        // Client-side vertex arrays.
        if (i32(this, 0x20) >= 0)
            glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (i32(this, 0x24) >= 0)
            glVertexAttribPointer(i32(this, 0x24), 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        if (i32(this, 0x28) >= 0)
            glVertexAttribPointer(i32(this, 0x28), 3, 0x1406, 0, 0, *(void **)(m + 0x10));
        if (i32(this, 0x2c) >= 0)
            glVertexAttribPointer(i32(this, 0x2c), 3, 0x1406, 0, 0, *(void **)(m + 0x14));
        if (i32(this, 0x30) >= 0)
            glVertexAttribPointer(i32(this, 0x30), 3, 0x1406, 0, 0, *(void **)(m + 0x18));
    } else {
        // VBO path: bind each buffer and use zero offset.
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x60));
        glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x68));
        glVertexAttribPointer(i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x6c));
        glVertexAttribPointer(i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x70));
        glVertexAttribPointer(i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x74));
        glVertexAttribPointer(i32(this, 0x30), 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- BumpShaderV2_8d6b8.cpp ----
namespace AbyssEngine {

// AbyssEngine::BumpShaderV2::BumpShaderV2()
BumpShaderV2::BumpShaderV2()
{
    void *volatile cookie = __stack_chk_guard;

    ShaderBaseStruct_ctor(this);

    // Install our vtable (points one slot past the RTTI/offset header).
    *(uint32_t *)this = (uint32_t)BumpShaderV2_vtable + 8;

    // Remember the global shader index assigned by the base ctor.
    BumpShaderV2_ShaderIndex = ShaderBaseStruct_shaderIndexIntern;

    // Name string lives at +0xc; build a temp and copy-assign it in.
    String tmp("BumpShaderV2", false);
    String_assign((String *)((char *)this + 0xc), &tmp);
    String_dtor(&tmp);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}

} // namespace AbyssEngine

// ---- Init_8d744.cpp ----
namespace AbyssEngine {

// AbyssEngine::BumpShaderV2::Init(AbyssEngine::Engine*)
void BumpShaderV2::Init(Engine *engine)
{
    (void)engine;

    int program = ShaderBaseStruct_ES2LoadProgram(this, "BumpShaderV2.vsh", "BumpShaderV2.fsh");
    i32(this, 0x04) = program;

    i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    i32(this, 0x24) = glGetAttribLocation(i32(this, 0x04), "a_normal");
    i32(this, 0x28) = glGetAttribLocation(i32(this, 0x04), "a_texCoord");
    i32(this, 0x2c) = glGetAttribLocation(i32(this, 0x04), "a_tangent");
    i32(this, 0x30) = glGetAttribLocation(i32(this, 0x04), "a_binormal");

    i32(this, 0x38) = glGetUniformLocation(i32(this, 0x04), "u_mvpMatrix");
    i32(this, 0x3c) = glGetUniformLocation(i32(this, 0x04), "u_normalMatrix");
    i32(this, 0x40) = glGetUniformLocation(i32(this, 0x04), "u_lightDir");
    i32(this, 0x44) = glGetUniformLocation(i32(this, 0x04), "u_eyePos");
    i32(this, 0x48) = glGetUniformLocation(i32(this, 0x04), "u_texture0");
    i32(this, 0x4c) = glGetUniformLocation(i32(this, 0x04), "u_texture1");
    i32(this, 0x50) = glGetUniformLocation(i32(this, 0x04), "u_ambient");
    i32(this, 0x54) = glGetUniformLocation(i32(this, 0x04), "u_diffuse");
    i32(this, 0x58) = glGetUniformLocation(i32(this, 0x04), "u_specular");
    i32(this, 0x5c) = glGetUniformLocation(i32(this, 0x04), "u_emissive");

    glUseProgram(i32(this, 0x04));
    for (int i = 0; i != 2; i++) {
        int loc = i32(this, i * 4 + 0x48);
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

} // namespace AbyssEngine
