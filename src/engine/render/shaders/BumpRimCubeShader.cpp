#include "gof2/engine/render/shaders/BumpRimCubeShader.h"
#include "gof2/externs.h"

extern "C" void glUniform3fv(int location, int count, const float *value);

namespace AbyssEngine {

void BumpRimCubeShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->u1Loc >= 0)
        glUniformMatrix4fv(this->u1Loc, 1, 0, (char *)engine + 0x104);
    if (this->u2Loc >= 0)
        glUniformMatrix3fv(this->u2Loc, 1, 0, (char *)engine + 0x204);
    if (this->u3Loc >= 0)
        glUniformMatrix4fv(this->u3Loc, 1, 0, (char *)engine + 0x144);
    if (this->u19Loc >= 0)
        glUniform1f(this->u19Loc, g_rimGlobalA);
    if (this->u20Loc >= 0)
        glUniform1f(this->u20Loc, g_rimGlobalB);

    if (this->dirty != 0) {
        glUniform3f(this->u4Loc, field_f32(engine, 0x330),
                    field_f32(engine, 0x334), field_f32(engine, 0x338));
        if (this->u6Loc >= 0)
            glUniform3f(this->u6Loc, field_f32(engine, 0x34c),
                        field_f32(engine, 0x350), field_f32(engine, 0x354));
        if (this->u10Loc >= 0)
            glUniform4fv(this->u10Loc, 1, (float *)((char *)engine + 0xd0));
        if (this->u11Loc >= 0)
            glUniform3fv(this->u11Loc, 1, (float *)((char *)engine + 0x2cc));
        if (this->u12Loc >= 0)
            glUniform3fv(this->u12Loc, 1, (float *)((char *)engine + 0x2fc));
        if (this->u13Loc >= 0)
            glUniform3fv(this->u13Loc, 1, (float *)((char *)engine + 0x2e4));
        if (this->u17Loc >= 0)
            glUniform1f(this->u17Loc, field_f32(engine, 0x2c8));
        if (this->u18Loc >= 0)
            glUniform3fv(this->u18Loc, 1, (float *)((char *)engine + 800));
        int loc88 = this->u22Loc;
        if (loc88 >= 0) {
            float *v = *(AEMath::Vector *)((char *)engine + 0x3f0);
            glUniform3fv(loc88, 1, v);
        }
        if (this->u24Loc >= 0)
            glUniform1f(this->u24Loc, field_f32(engine, 1000));
        if (this->u23Loc >= 0)
            glUniform1f(this->u23Loc, field_f32(engine, 0x3ec));
        if (this->u25Loc >= 0)
            glUniform1i(this->u25Loc, g_rimByteGlobal);
        if (this->u21Loc >= 0) {
            float v = 0.0f;
            int *m30 = (int *)field_ptr(mesh, 0x30);
            if (m30 != 0) {
                v = 1.0f;
                if (m30[9] == 0)
                    v = 0.0f;
            }
            glUniform1f(this->u21Loc, v);
        }
        if (field_i32(engine, 0x32c) >= 2) {
            glUniform3fv(this->u14Loc, 1, (float *)((char *)engine + 0x2d8));
            glUniform3fv(this->u15Loc, 1, (float *)((char *)engine + 0x308));
            glUniform3fv(this->u16Loc, 1, (float *)((char *)engine + 0x2f0));
            glUniform3f(this->u5Loc, field_f32(engine, 0x33c),
                        field_f32(engine, 0x340), field_f32(engine, 0x344));
        } else {
            glUniform3f(this->u14Loc, 0, 0, 0);
            glUniform3f(this->u15Loc, 0, 0, 0);
            glUniform3f(this->u16Loc, 0, 0, 0);
            glUniform3f(this->u5Loc, field_f32(engine, 0x33c),
                        field_f32(engine, 0x340), field_f32(engine, 0x344));
        }
        this->dirty = 0;
    }

    if (this->a0Loc >= 0)
        glEnableVertexAttribArray(this->a0Loc);
    if (this->a1Loc >= 0)
        glEnableVertexAttribArray(this->a1Loc);
    if (this->a2Loc >= 0)
        glEnableVertexAttribArray(this->a2Loc);
    if (this->a3Loc >= 0)
        glEnableVertexAttribArray(this->a3Loc);
    if (this->u0Loc >= 0)
        glEnableVertexAttribArray(this->u0Loc);

    if (field_u8(mesh, 0x5c) == 0) {
        if (this->a0Loc >= 0)
            glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (this->a1Loc >= 0)
            glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (this->a2Loc >= 0)
            glVertexAttribPointer(this->a2Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        if (this->a3Loc >= 0)
            glVertexAttribPointer(this->a3Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        if (this->u0Loc >= 0)
            glVertexAttribPointer(this->u0Loc, 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(this->a2Loc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x70));
        glVertexAttribPointer(this->a3Loc, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x74));
        glVertexAttribPointer(this->u0Loc, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

void _ZN11AbyssEngine17BumpRimCubeShaderD0Ev(
    AbyssEngine::BumpRimCubeShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

namespace AbyssEngine {

void BumpRimCubeShader::SetInActive()
{
    int loc;
    loc = this->a0Loc;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->a1Loc;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->a2Loc;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->a3Loc;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->u0Loc;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

namespace AbyssEngine {

int BumpRimCubeShader::ShaderIndex;

__attribute__((minsize)) BumpRimCubeShader::BumpRimCubeShader()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    this->field_0x0 = _ZTVN11AbyssEngine17BumpRimCubeShaderE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"BumpRimCubeShader";
    
}

} // namespace AbyssEngine

namespace AbyssEngine {

void BumpRimCubeShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->LoadBindShader("BumpRimCubeShader.vsh", "BumpRimCubeShader.fsh");
    this->field_0x4 = program;
    if (program == 0) {
        program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpRimCubeShader.vsh", "BumpRimCubeShader.fsh");
        this->field_0x4 = program;
    }

    this->a0Loc = glGetAttribLocation(program, "a0");
    this->a1Loc = glGetAttribLocation(this->field_0x4, "a1");
    this->a2Loc = glGetAttribLocation(this->field_0x4, "a2");
    this->a3Loc = glGetAttribLocation(this->field_0x4, "a3");

    this->u0Loc = glGetUniformLocation(this->field_0x4, "u0");
    this->u1Loc = glGetUniformLocation(this->field_0x4, "u1");
    this->u2Loc = glGetUniformLocation(this->field_0x4, "u2");
    this->u3Loc = glGetUniformLocation(this->field_0x4, "u3");
    this->u4Loc = glGetUniformLocation(this->field_0x4, "u4");
    this->u5Loc = glGetUniformLocation(this->field_0x4, "u5");
    this->u6Loc = glGetUniformLocation(this->field_0x4, "u6");
    this->u7Loc = glGetUniformLocation(this->field_0x4, "u7");
    this->u8Loc = glGetUniformLocation(this->field_0x4, "u8");
    this->u9Loc = glGetUniformLocation(this->field_0x4, "u9");
    this->u10Loc = glGetUniformLocation(this->field_0x4, "u10");
    this->u11Loc = glGetUniformLocation(this->field_0x4, "u11");
    this->u12Loc = glGetUniformLocation(this->field_0x4, "u12");
    this->u13Loc = glGetUniformLocation(this->field_0x4, "u13");
    this->u14Loc = glGetUniformLocation(this->field_0x4, "u14");
    this->u15Loc = glGetUniformLocation(this->field_0x4, "u15");
    this->u16Loc = glGetUniformLocation(this->field_0x4, "u16");
    this->u17Loc = glGetUniformLocation(this->field_0x4, "u17");
    this->u18Loc = glGetUniformLocation(this->field_0x4, "u18");
    this->u19Loc = glGetUniformLocation(this->field_0x4, "u19");
    this->u20Loc = glGetUniformLocation(this->field_0x4, "u20");
    this->u21Loc = glGetUniformLocation(this->field_0x4, "u21");
    this->u22Loc = glGetUniformLocation(this->field_0x4, "u22");
    this->u23Loc = glGetUniformLocation(this->field_0x4, "u23");
    this->u24Loc = glGetUniformLocation(this->field_0x4, "u24");
    this->u25Loc = glGetUniformLocation(this->field_0x4, "u25");

    glUseProgram(this->field_0x4);
    for (int i = 0; i != 2; i++) {
        int loc = (&this->u7Loc)[i];   // u7Loc, u8Loc (adjacent samplers)
        if (loc >= 0)
            glUniform1i(loc, i);
    }
    glUniform1i(this->u9Loc, 7);
}

} // namespace AbyssEngine
