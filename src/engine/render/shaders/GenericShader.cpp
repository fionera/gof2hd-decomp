#include "gof2/engine/render/shaders/GenericShader.h"
#include "gof2/platform/gl.h"

// GenericShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char GenericShader_vtable[];

// Registry slot the constructor links itself into (engine-global, resolved at link).
extern "C" void *GenericShader_registerSrc;
extern "C" void **GenericShader_registerDst;

namespace AbyssEngine {

// Compiles the GenericShader program and caches its attribute/uniform location handles,
// binding the three texture sampler uniforms (u4..u6) to texture units 0..2.
void GenericShader::Init(Engine *)
{
    this->program = this->ES2LoadProgram("GenericShader.vsh", "GenericShader.fsh");

    this->aPosition  = glGetAttribLocation(this->program, "a0");
    this->aTexCoord  = glGetAttribLocation(this->program, "a1");
    this->aNormal    = glGetAttribLocation(this->program, "a2");
    this->aTangent   = glGetAttribLocation(this->program, "a3");
    this->aBitangent = glGetAttribLocation(this->program, "a4");

    this->uMvpMatrix         = glGetUniformLocation(this->program, "u0");
    this->uNormalMatrix      = glGetUniformLocation(this->program, "u1");
    this->uLightPosition     = glGetUniformLocation(this->program, "u2");
    this->uEyePosition       = glGetUniformLocation(this->program, "u3");
    this->uTexture0          = glGetUniformLocation(this->program, "u4");
    this->uTexture1          = glGetUniformLocation(this->program, "u5");
    this->uTexture2          = glGetUniformLocation(this->program, "u6");
    this->uAmbientColor      = glGetUniformLocation(this->program, "u7");
    this->uDiffuseColor      = glGetUniformLocation(this->program, "u8");
    this->uSpecularColor     = glGetUniformLocation(this->program, "u9");
    this->uEmissiveColor     = glGetUniformLocation(this->program, "u10");
    this->uMaterialShininess = glGetUniformLocation(this->program, "u11");

    glUseProgram(this->program);
    // bind the three texture sampler uniforms u4,u5,u6 to texture units 0,1,2.
    int *samplers = &this->uTexture0;
    for (int i = 0; i != 3; i++) {
        if (samplers[i] >= 0)
            glUniform1i(samplers[i], i);
    }
}

void GenericShader::SetInActive()
{
    if (this->aPosition >= 0)
        glDisableVertexAttribArray(this->aPosition);
    if (this->aTexCoord >= 0)
        glDisableVertexAttribArray(this->aTexCoord);
    if (this->aNormal >= 0)
        glDisableVertexAttribArray(this->aNormal);
    if (this->aTangent >= 0)
        glDisableVertexAttribArray(this->aTangent);
    if (this->aBitangent >= 0)
        glDisableVertexAttribArray(this->aBitangent);
}

// The Engine/Mesh objects carry the per-frame matrices, light/material data and GL vertex
// buffers at fixed offsets; those are foreign objects (modelled in a later pass), read here
// as raw GL data by offset.
void GenericShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    if (this->uMvpMatrix >= 0)
        glUniformMatrix4fv(this->uMvpMatrix, 1, 0, (const float *)(e + 0x104));
    if (this->uNormalMatrix >= 0)
        glUniformMatrix3fv(this->uNormalMatrix, 1, 0, (const float *)(e + 0x204));

    if (this->dirty != 0) {
        if (this->uLightPosition >= 0)
            glUniform3f(this->uLightPosition, *(float *)(e + 0x330), *(float *)(e + 0x334), *(float *)(e + 0x338));
        if (this->uEyePosition >= 0)
            glUniform3f(this->uEyePosition, *(float *)(e + 0x34c), *(float *)(e + 0x350), *(float *)(e + 0x354));
        if (this->uAmbientColor >= 0)
            glUniform4fv(this->uAmbientColor, 1, (const float *)(e + 0xd0));
        if (this->uDiffuseColor >= 0)
            glUniform3fv(this->uDiffuseColor, 1, (const float *)(e + 0x2cc));
        if (this->uSpecularColor >= 0)
            glUniform3fv(this->uSpecularColor, 1, (const float *)(e + 0x2fc));
        if (this->uEmissiveColor >= 0)
            glUniform3fv(this->uEmissiveColor, 1, (const float *)(e + 0x2e4));
        if (this->uMaterialShininess >= 0)
            glUniform1f(this->uMaterialShininess, *(float *)(e + 0x2c8));
        this->dirty = 0;
    }

    if (this->aPosition >= 0)
        glEnableVertexAttribArray(this->aPosition);
    if (this->aTexCoord >= 0)
        glEnableVertexAttribArray(this->aTexCoord);
    if (this->aNormal >= 0)
        glEnableVertexAttribArray(this->aNormal);
    if (this->aTangent >= 0)
        glEnableVertexAttribArray(this->aTangent);
    if (this->aBitangent >= 0)
        glEnableVertexAttribArray(this->aBitangent);

    if (*(uint8_t *)(m + 0x5c) != 0) {
        glBindBuffer(0x8892, *(int *)(m + 0x60));
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(m + 0x68));
        glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(m + 0x6c));
        glVertexAttribPointer(this->aNormal, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(m + 0x70));
        glVertexAttribPointer(this->aTangent, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(m + 0x74));
        glVertexAttribPointer(this->aBitangent, 3, 0x1406, 0, 0, 0);
        return;
    }

    if (this->aPosition >= 0)
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
    if (this->aTexCoord >= 0)
        glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
    if (this->aNormal >= 0)
        glVertexAttribPointer(this->aNormal, 3, 0x1406, 0, 0, *(void **)(m + 0x10));
    if (this->aTangent >= 0)
        glVertexAttribPointer(this->aTangent, 3, 0x1406, 0, 0, *(void **)(m + 0x14));
    if (this->aBitangent >= 0)
        glVertexAttribPointer(this->aBitangent, 3, 0x1406, 0, 0, *(void **)(m + 0x18));
}

GenericShader::GenericShader()
{
    this->vtable = GenericShader_vtable + 8;
    // Register this shader into the global registry list.
    *GenericShader_registerDst = GenericShader_registerSrc;
    this->name.s = u"GenericShader";
}

} // namespace AbyssEngine
