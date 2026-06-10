#include "gof2/BumpShaderV2.h"


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
    if (this->field_0x20 >= 0)
        glDisableVertexAttribArray(this->field_0x20);
    if (this->field_0x24 >= 0)
        glDisableVertexAttribArray(this->field_0x24);
    if (this->field_0x28 >= 0)
        glDisableVertexAttribArray(this->field_0x28);
    if (this->field_0x2c >= 0)
        glDisableVertexAttribArray(this->field_0x2c);
    if (this->field_0x30 >= 0)
        glDisableVertexAttribArray(this->field_0x30);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8d8c8.cpp ----
namespace AbyssEngine {

// AbyssEngine::BumpShaderV2::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void BumpShaderV2::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *m = (char *)mesh;
    char *e = (char *)engine;

    if (this->field_0x38 >= 0)
        glUniformMatrix4fv(this->field_0x38, 1, 0, (const float *)(e + 0x104));
    if (this->field_0x3c >= 0)
        glUniformMatrix3fv(this->field_0x3c, 1, 0, (const float *)(e + 0x204));

    // Per-material lighting block, uploaded once (gated by the +0x9 dirty flag).
    if (this->field_0x9) {
        if (this->field_0x40 >= 0)
            glUniform3f(this->field_0x40, f32(e, 0x330), f32(e, 0x334), f32(e, 0x338));
        if (this->field_0x44 >= 0)
            glUniform3f(this->field_0x44, f32(e, 0x34c), f32(e, 0x350), f32(e, 0x354));
        if (this->field_0x50 >= 0)
            glUniform4fv(this->field_0x50, 1, (const float *)(e + 0xd0));
        if (this->field_0x54 >= 0)
            glUniform4fv(this->field_0x54, 1, (const float *)(e + 0x2a8));
        if (this->field_0x58 >= 0)
            glUniform4fv(this->field_0x58, 1, (const float *)(e + 0x298));
        if (this->field_0x5c >= 0)
            glUniform4fv(this->field_0x5c, 1, (const float *)(e + 0x2b8));
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

    if (u8(m, 0x5c) == 0) {
        // Client-side vertex arrays.
        if (this->field_0x20 >= 0)
            glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (this->field_0x24 >= 0)
            glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        if (this->field_0x28 >= 0)
            glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, *(void **)(m + 0x10));
        if (this->field_0x2c >= 0)
            glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, *(void **)(m + 0x14));
        if (this->field_0x30 >= 0)
            glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, *(void **)(m + 0x18));
    } else {
        // VBO path: bind each buffer and use zero offset.
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x60));
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x68));
        glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x6c));
        glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x70));
        glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x74));
        glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, 0);
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
    this->field_0x0 = BumpShaderV2_vtable + 8;

    // Remember the global shader index assigned by the base ctor.
    BumpShaderV2_ShaderIndex = ShaderBaseStruct_shaderIndexIntern;

    // Name string lives at +0xc; build a temp and copy-assign it in.
    String tmp;
    String_ctor_char(&tmp, "BumpShaderV2", false);
    String_assign(&this->field_0xc, &tmp);
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
    this->field_0x4 = program;

    this->field_0x20 = glGetAttribLocation(program, "a_position");
    this->field_0x24 = glGetAttribLocation(this->field_0x4, "a_normal");
    this->field_0x28 = glGetAttribLocation(this->field_0x4, "a_texCoord");
    this->field_0x2c = glGetAttribLocation(this->field_0x4, "a_tangent");
    this->field_0x30 = glGetAttribLocation(this->field_0x4, "a_binormal");

    this->field_0x38 = glGetUniformLocation(this->field_0x4, "u_mvpMatrix");
    this->field_0x3c = glGetUniformLocation(this->field_0x4, "u_normalMatrix");
    this->field_0x40 = glGetUniformLocation(this->field_0x4, "u_lightDir");
    this->field_0x44 = glGetUniformLocation(this->field_0x4, "u_eyePos");
    this->field_0x48 = glGetUniformLocation(this->field_0x4, "u_texture0");
    this->field_0x4c = glGetUniformLocation(this->field_0x4, "u_texture1");
    this->field_0x50 = glGetUniformLocation(this->field_0x4, "u_ambient");
    this->field_0x54 = glGetUniformLocation(this->field_0x4, "u_diffuse");
    this->field_0x58 = glGetUniformLocation(this->field_0x4, "u_specular");
    this->field_0x5c = glGetUniformLocation(this->field_0x4, "u_emissive");

    glUseProgram(this->field_0x4);
    for (int i = 0; i != 2; i++) {
        int loc = (&this->field_0x48)[i];
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

} // namespace AbyssEngine
