#include "gof2/game/core/BumpShaderCloak.h"
#include "gof2/platform/gl.h"

// Engine is the top-level (::Engine) host object; only the few entry points this shader calls
// are needed here. Its full layout/field reads are reached by raw offset (deferred pass).
class Engine {
public:
    uint32_t GetDisplayWidth();
    uint32_t GetDisplayHeight();
    void ActivateRefractFBO();
};

namespace AbyssEngine {

int BumpShaderCloak::ShaderIndex;

// AbyssEngine::BumpShaderCloak::BumpShaderCloak()
//   Installs the BumpShaderCloak vtable, publishes its shader index, and stores
//   the shader resource name.
BumpShaderCloak::BumpShaderCloak()
{
    this->vtable = (void *)(_ZTVN11AbyssEngine15BumpShaderCloakE + 8);
    BumpShaderCloak::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"BumpShaderCloak";
}

void BumpShaderCloak::Init(Engine *engine)
{
    int program = this->ES2LoadProgram("BumpShaderCloak.vsh", "BumpShaderCloak.fsh");
    this->program = program;

    this->attrib_a0 = glGetAttribLocation(program, "a0");
    this->attrib_a1 = glGetAttribLocation(this->program, "a1");
    this->attrib_a2 = glGetAttribLocation(this->program, "a2");
    this->attrib_a3 = glGetAttribLocation(this->program, "a3");
    this->attrib_a4 = glGetAttribLocation(this->program, "a4");

    this->uniform_u0 = glGetUniformLocation(this->program, "u0");
    this->uniform_u1 = glGetUniformLocation(this->program, "u1");
    this->uniform_u2 = glGetUniformLocation(this->program, "u2");
    this->uniform_u3 = glGetUniformLocation(this->program, "u3");
    this->uniform_u4 = glGetUniformLocation(this->program, "u4");
    this->uniform_u5 = glGetUniformLocation(this->program, "u5");
    this->uniform_u6 = glGetUniformLocation(this->program, "u6");
    this->uniform_u7 = glGetUniformLocation(this->program, "u7");
    this->uniform_u8 = glGetUniformLocation(this->program, "u8");
    this->uniform_u9 = glGetUniformLocation(this->program, "u9");
    this->uniform_u10 = glGetUniformLocation(this->program, "u10");
    this->uniform_u11 = glGetUniformLocation(this->program, "u11");
    this->uniform_u12 = glGetUniformLocation(this->program, "u12");
    this->uniform_u13 = glGetUniformLocation(this->program, "u13");
    this->uniform_u14 = glGetUniformLocation(this->program, "u14");
    this->uniform_u15 = glGetUniformLocation(this->program, "u15");
    this->uniform_u16 = glGetUniformLocation(this->program, "u16");
    this->uniform_u17 = glGetUniformLocation(this->program, "u17");

    glActiveTexture(0x84c7);
    ((::Engine *)engine)->ActivateRefractFBO();

    this->uniform_u18 = glGetUniformLocation(this->program, "u18");
    this->uniform_u19 = glGetUniformLocation(this->program, "u19");
    this->uniform_u20 = glGetUniformLocation(this->program, "u20");

    glUseProgram(this->program);
    glUniform1i(this->uniform_u5, 0);
    glUniform1i(this->uniform_u6, 1);
    glUniform1i(this->uniform_u7, 6);
    glUniform1i(this->uniform_u20, 7);
}

void BumpShaderCloak::SetInActive()
{
    if (this->attrib_a0 >= 0)
        glDisableVertexAttribArray(this->attrib_a0);
    if (this->attrib_a1 >= 0)
        glDisableVertexAttribArray(this->attrib_a1);
    if (this->attrib_a2 >= 0)
        glDisableVertexAttribArray(this->attrib_a2);
    if (this->attrib_a3 >= 0)
        glDisableVertexAttribArray(this->attrib_a3);
    if (this->attrib_a4 >= 0)
        glDisableVertexAttribArray(this->attrib_a4);
}

void BumpShaderCloak::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *m = (char *)mesh;
    char *e = (char *)engine;

    if (this->uniform_u0 >= 0)
        glUniformMatrix4fv(this->uniform_u0, 1, 0, (const float *)(e + 0x104));
    if (this->uniform_u1 >= 0)
        glUniformMatrix3fv(this->uniform_u1, 1, 0, (const float *)(e + 0x204));

    if (this->dirty) {
        glUniform3f(this->uniform_u2, field_f32(e, 0x330), field_f32(e, 0x334), field_f32(e, 0x338));
        if (this->uniform_u4 >= 0)
            glUniform3f(this->uniform_u4, field_f32(e, 0x34c), field_f32(e, 0x350), field_f32(e, 0x354));
        if (this->uniform_u8 >= 0)
            glUniform4fv(this->uniform_u8, 1, (const float *)(e + 0xd0));
        if (this->uniform_u9 >= 0)
            glUniform3fv(this->uniform_u9, 1, (const float *)(e + 0x2cc));
        if (this->uniform_u10 >= 0)
            glUniform3fv(this->uniform_u10, 1, (const float *)(e + 0x2fc));
        if (this->uniform_u11 >= 0)
            glUniform3fv(this->uniform_u11, 1, (const float *)(e + 0x2e4));
        if (this->uniform_u15 >= 0)
            glUniform1f(this->uniform_u15, field_f32(e, 0x2c8));
        if (this->uniform_u16 >= 0)
            glUniform3fv(this->uniform_u16, 1, (const float *)(e + 0x320));

        int viewportLoc = this->uniform_u17;
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

        if (this->uniform_u18 >= 0)
            glUniform1f(this->uniform_u18, field_f32(m, 0x1c));
        if (this->uniform_u19 >= 0)
            glUniform1f(this->uniform_u19, field_f32(m, 0x20));

        if (field_i32(e, 0x32c) < 2) {
            glUniform3f(this->uniform_u12, 0.0f, 0.0f, 0.0f);
            glUniform3f(this->uniform_u13, 0.0f, 0.0f, 0.0f);
            glUniform3f(this->uniform_u14, 0.0f, 0.0f, 0.0f);
            glUniform3f(this->uniform_u3, field_f32(e, 0x33c), field_f32(e, 0x340), field_f32(e, 0x344));
        } else {
            glUniform3fv(this->uniform_u12, 1, (const float *)(e + 0x2d8));
            glUniform3fv(this->uniform_u13, 1, (const float *)(e + 0x308));
            glUniform3fv(this->uniform_u14, 1, (const float *)(e + 0x2f0));
            glUniform3f(this->uniform_u3, field_f32(e, 0x33c), field_f32(e, 0x340), field_f32(e, 0x344));
        }
        this->dirty = 0;
    }

    if (this->attrib_a0 >= 0)
        glEnableVertexAttribArray(this->attrib_a0);
    if (this->attrib_a1 >= 0)
        glEnableVertexAttribArray(this->attrib_a1);
    if (this->attrib_a2 >= 0)
        glEnableVertexAttribArray(this->attrib_a2);
    if (this->attrib_a3 >= 0)
        glEnableVertexAttribArray(this->attrib_a3);
    if (this->attrib_a4 >= 0)
        glEnableVertexAttribArray(this->attrib_a4);

    if (field_u8(m, 0x5c) == 0) {
        if (this->attrib_a0 >= 0)
            glVertexAttribPointer(this->attrib_a0, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (this->attrib_a1 >= 0)
            glVertexAttribPointer(this->attrib_a1, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        if (this->attrib_a2 >= 0)
            glVertexAttribPointer(this->attrib_a2, 3, 0x1406, 0, 0, *(void **)(m + 0x10));
        if (this->attrib_a3 >= 0)
            glVertexAttribPointer(this->attrib_a3, 3, 0x1406, 0, 0, *(void **)(m + 0x14));
        if (this->attrib_a4 >= 0)
            glVertexAttribPointer(this->attrib_a4, 3, 0x1406, 0, 0, *(void **)(m + 0x18));
    } else {
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x60));
        glVertexAttribPointer(this->attrib_a0, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x68));
        glVertexAttribPointer(this->attrib_a1, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x6c));
        glVertexAttribPointer(this->attrib_a2, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x70));
        glVertexAttribPointer(this->attrib_a3, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x74));
        glVertexAttribPointer(this->attrib_a4, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
