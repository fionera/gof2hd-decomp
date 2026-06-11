#include "gof2/BumpShaderV2.h"
#include "gof2/String.h"


extern "C" int ShaderBaseStruct_shaderIndexIntern;
extern "C" int BumpShaderV2_ShaderIndex;

// ---- _BumpShaderV2_8dacc.cpp ----
void _ZN11AbyssEngine12BumpShaderV2D0Ev(BumpShaderV2 *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}

// ---- SetInActive_8d888.cpp ----
namespace AbyssEngine {

void BumpShaderV2::SetInActive()
{
    if (this->aPosition >= 0)
        glDisableVertexAttribArray(this->aPosition);
    if (this->aNormal >= 0)
        glDisableVertexAttribArray(this->aNormal);
    if (this->aTexCoord >= 0)
        glDisableVertexAttribArray(this->aTexCoord);
    if (this->aTangent >= 0)
        glDisableVertexAttribArray(this->aTangent);
    if (this->aBinormal >= 0)
        glDisableVertexAttribArray(this->aBinormal);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8d8c8.cpp ----
namespace AbyssEngine {

// AbyssEngine::BumpShaderV2::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void BumpShaderV2::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *m = (char *)mesh;
    char *e = (char *)engine;

    if (this->uMvpMatrix >= 0)
        glUniformMatrix4fv(this->uMvpMatrix, 1, 0, (const float *)(e + 0x104));
    if (this->uNormalMatrix >= 0)
        glUniformMatrix3fv(this->uNormalMatrix, 1, 0, (const float *)(e + 0x204));

    // Per-material lighting block, uploaded once (gated by the +0x9 dirty flag).
    if (this->lightingDirty) {
        if (this->uLightDir >= 0)
            glUniform3f(this->uLightDir, f32(e, 0x330), f32(e, 0x334), f32(e, 0x338));
        if (this->uEyePos >= 0)
            glUniform3f(this->uEyePos, f32(e, 0x34c), f32(e, 0x350), f32(e, 0x354));
        if (this->uAmbient >= 0)
            glUniform4fv(this->uAmbient, 1, (const float *)(e + 0xd0));
        if (this->uDiffuse >= 0)
            glUniform4fv(this->uDiffuse, 1, (const float *)(e + 0x2a8));
        if (this->uSpecular >= 0)
            glUniform4fv(this->uSpecular, 1, (const float *)(e + 0x298));
        if (this->uEmissive >= 0)
            glUniform4fv(this->uEmissive, 1, (const float *)(e + 0x2b8));
        this->lightingDirty = 0;
    }

    if (this->aPosition >= 0)
        glEnableVertexAttribArray(this->aPosition);
    if (this->aNormal >= 0)
        glEnableVertexAttribArray(this->aNormal);
    if (this->aTexCoord >= 0)
        glEnableVertexAttribArray(this->aTexCoord);
    if (this->aTangent >= 0)
        glEnableVertexAttribArray(this->aTangent);
    if (this->aBinormal >= 0)
        glEnableVertexAttribArray(this->aBinormal);

    if (u8(m, 0x5c) == 0) {
        // Client-side vertex arrays.
        if (this->aPosition >= 0)
            glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (this->aNormal >= 0)
            glVertexAttribPointer(this->aNormal, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        if (this->aTexCoord >= 0)
            glVertexAttribPointer(this->aTexCoord, 3, 0x1406, 0, 0, *(void **)(m + 0x10));
        if (this->aTangent >= 0)
            glVertexAttribPointer(this->aTangent, 3, 0x1406, 0, 0, *(void **)(m + 0x14));
        if (this->aBinormal >= 0)
            glVertexAttribPointer(this->aBinormal, 3, 0x1406, 0, 0, *(void **)(m + 0x18));
    } else {
        // VBO path: bind each buffer and use zero offset.
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x60));
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x68));
        glVertexAttribPointer(this->aNormal, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x6c));
        glVertexAttribPointer(this->aTexCoord, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x70));
        glVertexAttribPointer(this->aTangent, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x74));
        glVertexAttribPointer(this->aBinormal, 3, 0x1406, 0, 0, 0);
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
    this->vtable = BumpShaderV2_vtable + 8;

    // Remember the global shader index assigned by the base ctor.
    BumpShaderV2_ShaderIndex = ShaderBaseStruct_shaderIndexIntern;

    // Name string lives at +0xc; build a temp and copy-assign it in.
    String tmp;
    ((String *)(&tmp))->ctor_char("BumpShaderV2", false);
    ((String *)(&this->name))->assign(&tmp);
    ((String *)(&tmp))->dtor();

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
    this->program = program;

    this->aPosition = glGetAttribLocation(program, "a_position");
    this->aNormal = glGetAttribLocation(this->program, "a_normal");
    this->aTexCoord = glGetAttribLocation(this->program, "a_texCoord");
    this->aTangent = glGetAttribLocation(this->program, "a_tangent");
    this->aBinormal = glGetAttribLocation(this->program, "a_binormal");

    this->uMvpMatrix = glGetUniformLocation(this->program, "u_mvpMatrix");
    this->uNormalMatrix = glGetUniformLocation(this->program, "u_normalMatrix");
    this->uLightDir = glGetUniformLocation(this->program, "u_lightDir");
    this->uEyePos = glGetUniformLocation(this->program, "u_eyePos");
    this->uTexture0 = glGetUniformLocation(this->program, "u_texture0");
    this->uTexture1 = glGetUniformLocation(this->program, "u_texture1");
    this->uAmbient = glGetUniformLocation(this->program, "u_ambient");
    this->uDiffuse = glGetUniformLocation(this->program, "u_diffuse");
    this->uSpecular = glGetUniformLocation(this->program, "u_specular");
    this->uEmissive = glGetUniformLocation(this->program, "u_emissive");

    glUseProgram(this->program);
    for (int i = 0; i != 2; i++) {
        int loc = (&this->uTexture0)[i];
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

} // namespace AbyssEngine
