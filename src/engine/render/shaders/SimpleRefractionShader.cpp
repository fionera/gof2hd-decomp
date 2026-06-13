#include "gof2/engine/render/shaders/SimpleRefractionShader.h"
#include "gof2/engine/render/Engine.h"

extern "C" void glUniform2f(int location, float x, float y);

namespace AbyssEngine {

void SimpleRefractionShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->uM1Loc >= 0)
        glUniformMatrix4fv(this->uM1Loc, 1, 0, (char *)engine + 0x104);

    if (this->dirty != 0) {
        if (this->uM4Loc >= 0)
            glUniform1f(this->uM4Loc, -2.0f);
        if (this->uM3Loc >= 0)
            glUniform4fv(this->uM3Loc, 1, (float *)((char *)engine + 0xd0));
        if (this->uM2Loc >= 0)
            glUniform3f(this->uM2Loc, field_f32(engine, 0x34c), field_f32(engine, 0x350),
                        field_f32(engine, 0x354));
        int loc = this->uM5Loc;
        float w = (float)((::Engine *)engine)->GetDisplayWidth();
        float h = (float)((::Engine *)engine)->GetDisplayHeight();
        glUniform2f(loc, 1.0f / w, 1.0f / h);
        glActiveTexture(0x84c7);
        ((::Engine *)engine)->ActivateRefractFBO();
        glUniform1f(this->uRefractLoc, field_f32(mesh, 0x20));
        this->dirty = 0;
    }

    if (this->aPositionLoc >= 0)
        glEnableVertexAttribArray(this->aPositionLoc);
    if (this->aNormalLoc >= 0)
        glEnableVertexAttribArray(this->aNormalLoc);
    if (this->aTangentLoc >= 0)
        glEnableVertexAttribArray(this->aTangentLoc);
    if (this->aTexCoordLoc >= 0)
        glEnableVertexAttribArray(this->aTexCoordLoc);
    if (this->uM0Loc >= 0)
        glEnableVertexAttribArray(this->uM0Loc);

    if (field_u8(mesh, 0x5c) == 0) {
        if (this->aPositionLoc >= 0)
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (this->aNormalLoc >= 0)
            glVertexAttribPointer(this->aNormalLoc, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (this->aTangentLoc >= 0)
            glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        if (this->aTexCoordLoc >= 0)
            glVertexAttribPointer(this->aTexCoordLoc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        if (this->uM0Loc >= 0)
            glVertexAttribPointer(this->uM0Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(this->aNormalLoc, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x70));
        glVertexAttribPointer(this->aTexCoordLoc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x74));
        glVertexAttribPointer(this->uM0Loc, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

void _ZN11AbyssEngine22SimpleRefractionShaderD0Ev(
    AbyssEngine::SimpleRefractionShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

namespace AbyssEngine {

void SimpleRefractionShader::SetInActive()
{
    int loc;
    loc = this->aPositionLoc;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->aNormalLoc;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->aTangentLoc;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->aTexCoordLoc;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->uM0Loc;
    if (loc < 0)
        return;
    return glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

namespace AbyssEngine {

void SimpleRefractionShader::Init(Engine *engine)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram(
        "SimpleRefractionShader.vsh", "SimpleRefractionShader.fsh");
    this->field_0x4 = program;

    this->aPositionLoc = glGetAttribLocation(program, "a_position");
    this->aNormalLoc = glGetAttribLocation(this->field_0x4, "a_normal");
    this->aTangentLoc = glGetAttribLocation(this->field_0x4, "a_tangent");
    this->aTexCoordLoc = glGetAttribLocation(this->field_0x4, "a_texCoord");

    this->uM0Loc = glGetUniformLocation(this->field_0x4, "u_m0");
    this->uM1Loc = glGetUniformLocation(this->field_0x4, "u_m1");
    this->uM2Loc = glGetUniformLocation(this->field_0x4, "u_m2");
    this->uTex0Loc = glGetUniformLocation(this->field_0x4, "u_tex0");
    this->uTex1Loc = glGetUniformLocation(this->field_0x4, "u_tex1");
    this->uM3Loc = glGetUniformLocation(this->field_0x4, "u_m3");
    this->uM4Loc = glGetUniformLocation(this->field_0x4, "u_m4");
    this->uM5Loc = glGetUniformLocation(this->field_0x4, "u_m5");

    glActiveTexture(0x84c7);
    ((::Engine *)engine)->ActivateRefractFBO();

    this->uRefractLoc = glGetUniformLocation(this->field_0x4, "u_refract");
    this->uM6Loc = glGetUniformLocation(this->field_0x4, "u_m6");

    glUseProgram(this->field_0x4);

    int samplerLocs[2] = { this->uTex0Loc, this->uTex1Loc };
    for (int i = 0; i != 2; i = i + 1) {
        if (samplerLocs[i] >= 0)
            glUniform1i(samplerLocs[i], i);
    }
    return glUniform1i(this->uM6Loc, 7);
}

} // namespace AbyssEngine

extern "C" AbyssEngine::SimpleRefractionShader *
_ZN11AbyssEngine22SimpleRefractionShaderC2Ev(AbyssEngine::SimpleRefractionShader *self)
{
    new ((AbyssEngine::ShaderBaseStruct *)self) AbyssEngine::ShaderBaseStruct();
    self->field_0x0 = (void *)(_ZTVN11AbyssEngine22SimpleRefractionShaderE + 8);
    AbyssEngine::SimpleRefractionShader::ShaderIndex =
        AbyssEngine::ShaderBaseStruct::shaderIndexIntern;
    self->name = SimpleRefractionShader_name;
    return self;
}
