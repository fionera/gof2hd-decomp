#include "gof2/game/core/BumpShaderV3.h"
#include "gof2/engine/render/Mesh.h"
#include "gof2/platform/gl.h"

// externs.h references a global `Engine` (formerly supplied by the removed fwd.h); the engine
// type is AbyssEngine::Engine and externs only takes it by pointer, so an incomplete decl
// satisfies it here.
class Engine;
#include "gof2/externs.h"

// BumpShaderV3's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char _ZTVN11AbyssEngine12BumpShaderV3E[];

namespace AbyssEngine {

int BumpShaderV3::ShaderIndex;

BumpShaderV3::BumpShaderV3()
{
    this->vtable = (void *)(_ZTVN11AbyssEngine12BumpShaderV3E + 8);
    ShaderBaseStruct::shaderIndexIntern = ShaderIndex;
    this->name.s = u"BumpShaderV3";
}

void BumpShaderV3::Init(Engine *)
{
    this->program = this->ES2LoadProgram("BumpShaderV3.vsh", "BumpShaderV3.fsh");

    this->aPosition = glGetAttribLocation(this->program, "a0");
    this->aTexCoord = glGetAttribLocation(this->program, "a1");
    this->aNormal = glGetAttribLocation(this->program, "a2");
    this->aTangent = glGetAttribLocation(this->program, "a3");
    this->aBitangent = glGetAttribLocation(this->program, "a4");

    this->uModelViewProjectionMatrix = glGetUniformLocation(this->program, "u0");
    this->uModelMatrix = glGetUniformLocation(this->program, "u1");
    this->uLightDirModel0 = glGetUniformLocation(this->program, "u2");
    this->uLightDirModel1 = glGetUniformLocation(this->program, "u3");
    this->uEyePosModel = glGetUniformLocation(this->program, "u4");
    this->uTexDiffuse = glGetUniformLocation(this->program, "u5");
    this->uTexNormal = glGetUniformLocation(this->program, "u6");
    this->uTexSpecular = glGetUniformLocation(this->program, "u7");
    this->uGlColor = glGetUniformLocation(this->program, "u8");
    this->uAmbientColor0 = glGetUniformLocation(this->program, "u9");
    this->uAmbientColor1 = glGetUniformLocation(this->program, "u10");
    this->uDiffuseColor1 = glGetUniformLocation(this->program, "u11");
    this->uDiffuseColor0 = glGetUniformLocation(this->program, "u12");
    this->uSpecularColor0 = glGetUniformLocation(this->program, "u13");
    this->uSpecularColor1 = glGetUniformLocation(this->program, "u14");
    this->uSpecularPower = glGetUniformLocation(this->program, "u15");
    this->uRimColor = glGetUniformLocation(this->program, "u16");
    this->uTexBiasDiffuse = glGetUniformLocation(this->program, "u17");
    this->uTexBiasNormal = glGetUniformLocation(this->program, "u18");
    this->uIsGlowMat = glGetUniformLocation(this->program, "u19");

    glUseProgram(this->program);
    // bind the three sampler uniforms (u5/u6/u7) to texture units 0/1/2.
    int textureUniforms[3] = { this->uTexDiffuse, this->uTexNormal, this->uTexSpecular };
    for (int i = 0; i != 3; i++) {
        int loc = textureUniforms[i];
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

void BumpShaderV3::SetInActive()
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

void BumpShaderV3::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->uModelViewProjectionMatrix >= 0)
        glUniformMatrix4fv(this->uModelViewProjectionMatrix, 1, 0, (float *)((char *)engine + 0x104));
    if (this->uModelMatrix >= 0)
        glUniformMatrix3fv(this->uModelMatrix, 1, 0, (float *)((char *)engine + 0x204));
    if (this->uTexBiasDiffuse >= 0)
        glUniform1f(this->uTexBiasDiffuse, *g_bsv3_floatA);
    if (this->uTexBiasNormal >= 0)
        glUniform1f(this->uTexBiasNormal, *g_bsv3_floatB);

    if (this->dirty != 0) {
        glUniform3f(this->uLightDirModel0, field_f32(engine, 0x330), field_f32(engine, 0x334),
                    field_f32(engine, 0x338));
        if (this->uEyePosModel >= 0)
            glUniform3f(this->uEyePosModel, field_f32(engine, 0x34c), field_f32(engine, 0x350),
                        field_f32(engine, 0x354));
        if (this->uGlColor >= 0)
            glUniform4fv(this->uGlColor, 1, (float *)((char *)engine + 0xd0));
        if (this->uAmbientColor0 >= 0)
            glUniform3fv(this->uAmbientColor0, 1, (float *)((char *)engine + 0x2cc));
        if (this->uAmbientColor1 >= 0)
            glUniform3fv(this->uAmbientColor1, 1, (float *)((char *)engine + 0x2fc));
        if (this->uDiffuseColor1 >= 0)
            glUniform3fv(this->uDiffuseColor1, 1, (float *)((char *)engine + 0x2e4));
        if (this->uSpecularPower >= 0)
            glUniform1f(this->uSpecularPower, field_f32(engine, 0x2c8));
        if (this->uRimColor >= 0)
            glUniform3fv(this->uRimColor, 1, (float *)((char *)engine + 0x320));
        if (this->uIsGlowMat >= 0) {
            float w = 0.85f;
            if (*(int *)((char *)mesh->field_0x30 + 0x24) != 0)
                w = 1.0f;
            glUniform1f(this->uIsGlowMat, w);
        }
        if (field_i32(engine, 0x32c) >= 2) {
            glUniform3fv(this->uDiffuseColor0, 1, (float *)((char *)engine + 0x2d8));
            glUniform3fv(this->uSpecularColor0, 1, (float *)((char *)engine + 0x308));
            glUniform3fv(this->uSpecularColor1, 1, (float *)((char *)engine + 0x2f0));
            glUniform3f(this->uLightDirModel1, field_f32(engine, 0x33c), field_f32(engine, 0x340),
                        field_f32(engine, 0x344));
        } else {
            glUniform3f(this->uDiffuseColor0, 0, 0, 0);
            glUniform3f(this->uSpecularColor0, 0, 0, 0);
            glUniform3f(this->uSpecularColor1, 0, 0, 0);
            glUniform3f(this->uLightDirModel1, field_f32(engine, 0x33c), field_f32(engine, 0x340),
                        field_f32(engine, 0x344));
        }
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

    if (field_u8(mesh, 0x5c) == 0) {
        if (this->aPosition >= 0)
            glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (this->aTexCoord >= 0)
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (this->aNormal >= 0)
            glVertexAttribPointer(this->aNormal, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        if (this->aTangent >= 0)
            glVertexAttribPointer(this->aTangent, 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        if (this->aBitangent >= 0)
            glVertexAttribPointer(this->aBitangent, 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(this->aNormal, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x70));
        glVertexAttribPointer(this->aTangent, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x74));
        glVertexAttribPointer(this->aBitangent, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
