#include "gof2/game/core/BumpShaderV2.h"
#include "gof2/platform/gl.h"

// Cross-object reads of Engine/Mesh are deferred to a later Engine/Mesh-modeling pass; until
// then their fields are reached by raw offset.
template <class T> static inline T &ae_field(void *base, unsigned int off) {
    return *(T *)((char *)base + off);
}

// BumpShaderV2's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char BumpShaderV2_vtable;

// Running global shader index, snapshotted by the base ctor.
extern "C" int BumpShaderV2_ShaderIndex;

namespace AbyssEngine {

BumpShaderV2::BumpShaderV2()
{
    this->vtable = &BumpShaderV2_vtable + 8;
    BumpShaderV2_ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"BumpShaderV2";
}

// Compiles the GLES2 program and caches its attribute/uniform locations.
void BumpShaderV2::Init(Engine *)
{
    this->program = this->ES2LoadProgram("BumpShaderV2.vsh", "BumpShaderV2.fsh");

    this->aPosition = glGetAttribLocation(this->program, "a_position");
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

// Uploads the per-frame matrices, per-material lighting block (gated by the dirty flag), and
// binds the mesh's vertex attribute arrays (client-side or VBO).
void BumpShaderV2::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->uMvpMatrix >= 0)
        glUniformMatrix4fv(this->uMvpMatrix, 1, 0, (const float *)((char *)engine + 0x104));
    if (this->uNormalMatrix >= 0)
        glUniformMatrix3fv(this->uNormalMatrix, 1, 0, (const float *)((char *)engine + 0x204));

    if (this->dirty) {
        if (this->uLightDir >= 0)
            glUniform3f(this->uLightDir, ae_field<float>(engine, 0x330),
                        ae_field<float>(engine, 0x334), ae_field<float>(engine, 0x338));
        if (this->uEyePos >= 0)
            glUniform3f(this->uEyePos, ae_field<float>(engine, 0x34c),
                        ae_field<float>(engine, 0x350), ae_field<float>(engine, 0x354));
        if (this->uAmbient >= 0)
            glUniform4fv(this->uAmbient, 1, (const float *)((char *)engine + 0xd0));
        if (this->uDiffuse >= 0)
            glUniform4fv(this->uDiffuse, 1, (const float *)((char *)engine + 0x2a8));
        if (this->uSpecular >= 0)
            glUniform4fv(this->uSpecular, 1, (const float *)((char *)engine + 0x298));
        if (this->uEmissive >= 0)
            glUniform4fv(this->uEmissive, 1, (const float *)((char *)engine + 0x2b8));
        this->dirty = 0;
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

    if (ae_field<uint8_t>(mesh, 0x5c) == 0) {
        // Client-side vertex arrays.
        if (this->aPosition >= 0)
            glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, ae_field<void *>(mesh, 0x4));
        if (this->aNormal >= 0)
            glVertexAttribPointer(this->aNormal, 2, 0x1406, 0, 0, ae_field<void *>(mesh, 0x8));
        if (this->aTexCoord >= 0)
            glVertexAttribPointer(this->aTexCoord, 3, 0x1406, 0, 0, ae_field<void *>(mesh, 0x10));
        if (this->aTangent >= 0)
            glVertexAttribPointer(this->aTangent, 3, 0x1406, 0, 0, ae_field<void *>(mesh, 0x14));
        if (this->aBinormal >= 0)
            glVertexAttribPointer(this->aBinormal, 3, 0x1406, 0, 0, ae_field<void *>(mesh, 0x18));
    } else {
        // VBO path: bind each buffer and use zero offset.
        glBindBuffer(0x8892, ae_field<unsigned int>(mesh, 0x60));
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, ae_field<unsigned int>(mesh, 0x68));
        glVertexAttribPointer(this->aNormal, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, ae_field<unsigned int>(mesh, 0x6c));
        glVertexAttribPointer(this->aTexCoord, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, ae_field<unsigned int>(mesh, 0x70));
        glVertexAttribPointer(this->aTangent, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, ae_field<unsigned int>(mesh, 0x74));
        glVertexAttribPointer(this->aBinormal, 3, 0x1406, 0, 0, 0);
    }
}

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
