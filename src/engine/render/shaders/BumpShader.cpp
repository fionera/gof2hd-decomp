#include "gof2/engine/render/shaders/BumpShader.h"
#include "gof2/platform/gl.h"

// BumpShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char BumpShader_vtable;

// PC-relative pointer-to-pointer globals holding single floats / a byte (engine timers and
// a feature flag) read by UpdateMeshData.
extern "C" float *const gBumpFloatA;
extern "C" float *const gBumpFloatB;
extern "C" uint8_t *const gBumpFlag;

// Reinterprets an AEMath::Vector as its leading float[3]. Modelled in a later pass; here it is
// the engine helper reached through the ABI.
extern "C" float *Vector_cast_to_float(AbyssEngine::AEMath::Vector *self);

namespace AbyssEngine {

BumpShader::BumpShader()
{
    this->vtable = &BumpShader_vtable + 8;
    this->name.s = u"BumpShader";
}

// Sets the sampler uniform at `location` to texture unit `unit`.
void BumpShader::setSampler(int location, int unit)
{
    glUniform1i(location, unit);
}

void BumpShader::SetInActive()
{
    if (this->a0Loc >= 0)
        glDisableVertexAttribArray(this->a0Loc);
    if (this->a1Loc >= 0)
        glDisableVertexAttribArray(this->a1Loc);
    if (this->a2Loc >= 0)
        glDisableVertexAttribArray(this->a2Loc);
}

// Loads the program, then resolves every attribute/uniform location, activates the program
// and binds sampler u5=0, sampler u6=7.
void BumpShader::Init(Engine *)
{
    this->program = this->ES2LoadProgram("BumpShader.vsh", "BumpShader.fsh");

    this->a0Loc = glGetAttribLocation(this->program, "a0");
    this->a1Loc = glGetAttribLocation(this->program, "a1");
    this->a2Loc = glGetAttribLocation(this->program, "a2");

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
    this->u10Loc = glGetUniformLocation(this->program, "u10");
    this->u11Loc = glGetUniformLocation(this->program, "u11");
    this->u12Loc = glGetUniformLocation(this->program, "u12");
    this->u13Loc = glGetUniformLocation(this->program, "u13");
    this->u14Loc = glGetUniformLocation(this->program, "u14");
    this->u15Loc = glGetUniformLocation(this->program, "u15");
    this->u16Loc = glGetUniformLocation(this->program, "u16");
    this->u17Loc = glGetUniformLocation(this->program, "u17");
    this->u18Loc = glGetUniformLocation(this->program, "u18");
    this->u19Loc = glGetUniformLocation(this->program, "u19");
    this->u20Loc = glGetUniformLocation(this->program, "u20");
    this->u21Loc = glGetUniformLocation(this->program, "u21");

    glUseProgram(this->program);
    glUniform1i(this->u5Loc, 0);
    BumpShader::setSampler(this->u6Loc, 7);
}

// Pushes the per-frame matrices/lighting uniforms (only the first time per frame, gated by
// the dirty byte) and binds the mesh's vertex attribute streams.
void BumpShader::UpdateMeshData(Mesh *mesh, Engine *ctx)
{
    char *e = (char *)ctx;
    char *m = (char *)mesh;

    if (this->u0Loc >= 0)
        glUniformMatrix4fv(this->u0Loc, 1, 0, (const float *)(e + 0x104));
    if (this->u1Loc >= 0)
        glUniformMatrix3fv(this->u1Loc, 1, 0, (const float *)(e + 0x204));
    if (this->u16Loc >= 0)
        glUniform1f(this->u16Loc, *gBumpFloatA);
    if (this->u17Loc >= 0)
        glUniform1f(this->u17Loc, *gBumpFloatB);

    if (this->dirty != 0) {
        glUniform3f(this->u2Loc, *(float *)(e + 0x330), *(float *)(e + 0x334),
                    *(float *)(e + 0x338));
        if (this->u4Loc >= 0)
            glUniform3f(this->u4Loc, *(float *)(e + 0x34c), *(float *)(e + 0x350),
                        *(float *)(e + 0x354));
        if (this->u7Loc >= 0)
            glUniform4fv(this->u7Loc, 1, (const float *)(e + 0xd0));
        if (this->u8Loc >= 0)
            glUniform3fv(this->u8Loc, 1, (const float *)(e + 0x2cc));
        if (this->u9Loc >= 0)
            glUniform3fv(this->u9Loc, 1, (const float *)(e + 0x2fc));
        if (this->u10Loc >= 0)
            glUniform3fv(this->u10Loc, 1, (const float *)(e + 0x2e4));
        if (this->u14Loc >= 0)
            glUniform1f(this->u14Loc, *(float *)(e + 0x2c8));
        if (this->u15Loc >= 0)
            glUniform3fv(this->u15Loc, 1, (const float *)(e + 0x320));
        if (this->u18Loc >= 0) {
            float *v = Vector_cast_to_float((AEMath::Vector *)(e + 0x3f0));
            glUniform3fv(this->u18Loc, 1, v);
        }
        if (this->u20Loc >= 0)
            glUniform1f(this->u20Loc, *(float *)(e + 0x3e8));
        if (this->u19Loc >= 0)
            glUniform1f(this->u19Loc, *(float *)(e + 0x3ec));
        if (this->u21Loc >= 0)
            glUniform1i(this->u21Loc, *gBumpFlag);

        if (*(int *)(e + 0x32c) < 2) {
            // single-light path: zero the matrix uniforms, set the light dir.
            glUniform3f(this->u11Loc, 0, 0, 0);
            glUniform3f(this->u12Loc, 0, 0, 0);
            glUniform3f(this->u13Loc, 0, 0, 0);
            glUniform3f(this->u3Loc, *(float *)(e + 0x33c), *(float *)(e + 0x340),
                        *(float *)(e + 0x344));
        } else {
            glUniform3fv(this->u11Loc, 1, (const float *)(e + 0x2d8));
            glUniform3fv(this->u12Loc, 1, (const float *)(e + 0x308));
            glUniform3fv(this->u13Loc, 1, (const float *)(e + 0x2f0));
            glUniform3f(this->u3Loc, *(float *)(e + 0x33c), *(float *)(e + 0x340),
                        *(float *)(e + 0x344));
        }
        this->dirty = 0;
    }

    if (this->a0Loc >= 0)
        glEnableVertexAttribArray(this->a0Loc);
    if (this->a1Loc >= 0)
        glEnableVertexAttribArray(this->a1Loc);
    if (this->a2Loc >= 0)
        glEnableVertexAttribArray(this->a2Loc);

    if (u8(mesh, 0x5c) == 0) {
        if (this->a0Loc >= 0)
            glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (this->a1Loc >= 0)
            glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        if (this->a2Loc >= 0)
            glVertexAttribPointer(this->a2Loc, 3, 0x1406, 0, 0, *(void **)(m + 0x10));
    } else {
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x60));
        glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x68));
        glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x6c));
        glVertexAttribPointer(this->a2Loc, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
