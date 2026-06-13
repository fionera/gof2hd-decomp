#include "gof2/game/core/BumpShaderCloak.h"
#include "gof2/engine/render/Engine.h"

extern "C" void glUniform2f(int location, float x, float y);
extern "C" void glUniformMatrix3fv(int location, int count, uint8_t transpose, const void *value);

// ---- _BumpShaderCloak_8b200.cpp ----
void _ZN11AbyssEngine15BumpShaderCloakD0Ev(
    AbyssEngine::BumpShaderCloak *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

// ---- Init_8ac7c.cpp ----
namespace AbyssEngine {

void BumpShaderCloak::Init(Engine *engine)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpShaderCloak.vsh", "BumpShaderCloak.fsh");
    field_i32(this, 0x04) = program;

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
    field_i32(this, 0x58) = glGetUniformLocation(field_i32(this, 0x04), "u8");
    field_i32(this, 0x5c) = glGetUniformLocation(field_i32(this, 0x04), "u9");
    field_i32(this, 0x64) = glGetUniformLocation(field_i32(this, 0x04), "u10");
    field_i32(this, 0x6c) = glGetUniformLocation(field_i32(this, 0x04), "u11");
    field_i32(this, 0x60) = glGetUniformLocation(field_i32(this, 0x04), "u12");
    field_i32(this, 0x68) = glGetUniformLocation(field_i32(this, 0x04), "u13");
    field_i32(this, 0x70) = glGetUniformLocation(field_i32(this, 0x04), "u14");
    field_i32(this, 0x74) = glGetUniformLocation(field_i32(this, 0x04), "u15");
    field_i32(this, 0x78) = glGetUniformLocation(field_i32(this, 0x04), "u16");
    field_i32(this, 0x8c) = glGetUniformLocation(field_i32(this, 0x04), "u17");

    glActiveTexture(0x84c7);
    ((::Engine *)engine)->ActivateRefractFBO();

    field_i32(this, 0x80) = glGetUniformLocation(field_i32(this, 0x04), "u18");
    field_i32(this, 0x84) = glGetUniformLocation(field_i32(this, 0x04), "u19");
    field_i32(this, 0x90) = glGetUniformLocation(field_i32(this, 0x04), "u20");

    glUseProgram(field_i32(this, 0x04));
    void (*uniform1i)(int, int) = glUniform1i;
    uniform1i(field_i32(this, 0x48), 0);
    uniform1i(field_i32(this, 0x4c), 1);
    uniform1i(field_i32(this, 0x50), 6);
    return uniform1i(field_i32(this, 0x90), 7);
}

} // namespace AbyssEngine

// ---- SetInActive_8ae98.cpp ----
namespace AbyssEngine {

void BumpShaderCloak::SetInActive()
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

// ---- BumpShaderCloak_8abf0.cpp ----
namespace AbyssEngine {

int BumpShaderCloak::ShaderIndex;

// AbyssEngine::BumpShaderCloak::BumpShaderCloak()
BumpShaderCloak::BumpShaderCloak()
{

    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    *(void **)this = (void *)(_ZTVN11AbyssEngine15BumpShaderCloakE + 8);
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;

    String tmp("BumpShaderCloak");
    this->field_0xc().assign(&tmp);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8aed8.cpp ----
namespace AbyssEngine {

// AbyssEngine::BumpShaderCloak::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void BumpShaderCloak::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *m = (char *)mesh;
    char *e = (char *)engine;

    if (field_i32(this, 0x34) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, e + 0x104);
    if (field_i32(this, 0x38) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x38), 1, 0, e + 0x204);

    if (field_u8(this, 0x9)) {
        glUniform3f(field_i32(this, 0x3c), field_f32(e, 0x330), field_f32(e, 0x334), field_f32(e, 0x338));
        if (field_i32(this, 0x44) >= 0)
            glUniform3f(field_i32(this, 0x44), field_f32(e, 0x34c), field_f32(e, 0x350), field_f32(e, 0x354));
        if (field_i32(this, 0x58) >= 0)
            glUniform4fv(field_i32(this, 0x58), 1, (const float *)(e + 0xd0));
        if (field_i32(this, 0x5c) >= 0)
            glUniform3fv(field_i32(this, 0x5c), 1, (const float *)(e + 0x2cc));
        if (field_i32(this, 0x64) >= 0)
            glUniform3fv(field_i32(this, 0x64), 1, (const float *)(e + 0x2fc));
        if (field_i32(this, 0x6c) >= 0)
            glUniform3fv(field_i32(this, 0x6c), 1, (const float *)(e + 0x2e4));
        if (field_i32(this, 0x74) >= 0)
            glUniform1f(field_i32(this, 0x74), field_f32(e, 0x2c8));
        if (field_i32(this, 0x78) >= 0)
            glUniform3fv(field_i32(this, 0x78), 1, (const float *)(e + 0x320));

        int viewportLoc = field_i32(this, 0x8c);
        if (viewportLoc >= 0) {
            // The render-target's mode (at [[engine+0x30]+0x30]) picks portrait vs landscape.
            float invW, invH;
            if (*(int *)(*(char **)(e + 0x30) + 0x30) == 2) {
                invW = 1.0f / (float)((::Engine *)engine)->GetDisplayWidth();
                invH = 1.0f / (float)((::Engine *)engine)->GetDisplayHeight();
            } else {
                invW = 1.0f / (float)((::Engine *)engine)->GetDisplayHeight();
                invH = 1.0f / (float)((::Engine *)engine)->GetDisplayWidth();
            }
            glUniform2f(viewportLoc, invW, invH);
        }

        glActiveTexture(0x84c7);
        ((::Engine *)engine)->ActivateRefractFBO();

        if (field_i32(this, 0x80) >= 0)
            glUniform1f(field_i32(this, 0x80), field_f32(m, 0x1c));
        if (field_i32(this, 0x84) >= 0)
            glUniform1f(field_i32(this, 0x84), field_f32(m, 0x20));

        if (field_i32(e, 0x32c) < 2) {
            glUniform3f(field_i32(this, 0x60), 0.0f, 0.0f, 0.0f);
            glUniform3f(field_i32(this, 0x68), 0.0f, 0.0f, 0.0f);
            glUniform3f(field_i32(this, 0x70), 0.0f, 0.0f, 0.0f);
            glUniform3f(field_i32(this, 0x40), field_f32(e, 0x33c), field_f32(e, 0x340), field_f32(e, 0x344));
        } else {
            glUniform3fv(field_i32(this, 0x60), 1, (const float *)(e + 0x2d8));
            glUniform3fv(field_i32(this, 0x68), 1, (const float *)(e + 0x308));
            glUniform3fv(field_i32(this, 0x70), 1, (const float *)(e + 0x2f0));
            glUniform3f(field_i32(this, 0x40), field_f32(e, 0x33c), field_f32(e, 0x340), field_f32(e, 0x344));
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

    if (field_u8(m, 0x5c) == 0) {
        if (field_i32(this, 0x20) >= 0)
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (field_i32(this, 0x24) >= 0)
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        if (field_i32(this, 0x28) >= 0)
            glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, *(void **)(m + 0x10));
        if (field_i32(this, 0x2c) >= 0)
            glVertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, *(void **)(m + 0x14));
        if (field_i32(this, 0x30) >= 0)
            glVertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, *(void **)(m + 0x18));
    } else {
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x60));
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x68));
        glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x6c));
        glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x70));
        glVertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x74));
        glVertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
