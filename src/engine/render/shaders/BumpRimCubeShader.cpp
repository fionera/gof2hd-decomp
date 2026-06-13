#include "gof2/BumpRimCubeShader.h"
#include "gof2/externs.h"


extern "C" void glUniform3fv(int location, int count, const float *value);

// ---- UpdateMeshData_8fe0c.cpp ----
namespace AbyssEngine {

void BumpRimCubeShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->field_0x34 >= 0)
        glUniformMatrix4fv(this->field_0x34, 1, 0, (char *)engine + 0x104);
    if (this->field_0x38 >= 0)
        glUniformMatrix3fv(this->field_0x38, 1, 0, (char *)engine + 0x204);
    if (this->field_0x3c >= 0)
        glUniformMatrix4fv(this->field_0x3c, 1, 0, (char *)engine + 0x144);
    if (this->field_0x7c >= 0)
        glUniform1f(this->field_0x7c, g_rimGlobalA);
    if (this->field_0x80 >= 0)
        glUniform1f(this->field_0x80, g_rimGlobalB);

    if (this->field_0x9 != 0) {
        glUniform3f(this->field_0x40, field_f32(engine, 0x330),
                    field_f32(engine, 0x334), field_f32(engine, 0x338));
        if (this->field_0x48 >= 0)
            glUniform3f(this->field_0x48, field_f32(engine, 0x34c),
                        field_f32(engine, 0x350), field_f32(engine, 0x354));
        if (this->field_0x58 >= 0)
            glUniform4fv(this->field_0x58, 1, (float *)((char *)engine + 0xd0));
        if (this->field_0x5c >= 0)
            glUniform3fv(this->field_0x5c, 1, (float *)((char *)engine + 0x2cc));
        if (this->field_0x64 >= 0)
            glUniform3fv(this->field_0x64, 1, (float *)((char *)engine + 0x2fc));
        if (this->field_0x6c >= 0)
            glUniform3fv(this->field_0x6c, 1, (float *)((char *)engine + 0x2e4));
        if (this->field_0x74 >= 0)
            glUniform1f(this->field_0x74, field_f32(engine, 0x2c8));
        if (this->field_0x78 >= 0)
            glUniform3fv(this->field_0x78, 1, (float *)((char *)engine + 800));
        int loc88 = this->field_0x88;
        if (loc88 >= 0) {
            float *v = *(AEMath::Vector *)((char *)engine + 0x3f0);
            glUniform3fv(loc88, 1, v);
        }
        if (this->field_0x90 >= 0)
            glUniform1f(this->field_0x90, field_f32(engine, 1000));
        if (this->field_0x8c >= 0)
            glUniform1f(this->field_0x8c, field_f32(engine, 0x3ec));
        if (this->field_0x94 >= 0)
            glUniform1i(this->field_0x94, g_rimByteGlobal);
        if (this->field_0x84 >= 0) {
            float v = 0.0f;
            int *m30 = (int *)field_ptr(mesh, 0x30);
            if (m30 != 0) {
                v = 1.0f;
                if (m30[9] == 0)
                    v = 0.0f;
            }
            glUniform1f(this->field_0x84, v);
        }
        if (field_i32(engine, 0x32c) >= 2) {
            glUniform3fv(this->field_0x60, 1, (float *)((char *)engine + 0x2d8));
            glUniform3fv(this->field_0x68, 1, (float *)((char *)engine + 0x308));
            glUniform3fv(this->field_0x70, 1, (float *)((char *)engine + 0x2f0));
            glUniform3f(this->field_0x44, field_f32(engine, 0x33c),
                        field_f32(engine, 0x340), field_f32(engine, 0x344));
        } else {
            glUniform3f(this->field_0x60, 0, 0, 0);
            glUniform3f(this->field_0x68, 0, 0, 0);
            glUniform3f(this->field_0x70, 0, 0, 0);
            glUniform3f(this->field_0x44, field_f32(engine, 0x33c),
                        field_f32(engine, 0x340), field_f32(engine, 0x344));
        }
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

    if (field_u8(mesh, 0x5c) == 0) {
        if (this->field_0x20 >= 0)
            glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (this->field_0x24 >= 0)
            glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (this->field_0x28 >= 0)
            glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        if (this->field_0x2c >= 0)
            glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        if (this->field_0x30 >= 0)
            glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x70));
        glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x74));
        glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- _BumpRimCubeShader_90160.cpp ----
void _ZN11AbyssEngine17BumpRimCubeShaderD0Ev(
    AbyssEngine::BumpRimCubeShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

// ---- SetInActive_8fdcc.cpp ----
namespace AbyssEngine {

void BumpRimCubeShader::SetInActive()
{
    int loc;
    loc = this->field_0x20;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x24;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x28;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x2c;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->field_0x30;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

// ---- BumpRimCubeShader_8fad4.cpp ----
namespace AbyssEngine {

int BumpRimCubeShader::ShaderIndex;

__attribute__((minsize)) BumpRimCubeShader::BumpRimCubeShader()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    this->field_0x0 = _ZTVN11AbyssEngine17BumpRimCubeShaderE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->field_0xc.s = u"BumpRimCubeShader";
    
}

} // namespace AbyssEngine

// ---- Init_8fb60.cpp ----
namespace AbyssEngine {

void BumpRimCubeShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->LoadBindShader("BumpRimCubeShader.vsh", "BumpRimCubeShader.fsh");
    this->field_0x4 = program;
    if (program == 0) {
        program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpRimCubeShader.vsh", "BumpRimCubeShader.fsh");
        this->field_0x4 = program;
    }

    this->field_0x20 = glGetAttribLocation(program, "a0");
    this->field_0x24 = glGetAttribLocation(this->field_0x4, "a1");
    this->field_0x28 = glGetAttribLocation(this->field_0x4, "a2");
    this->field_0x2c = glGetAttribLocation(this->field_0x4, "a3");

    this->field_0x30 = glGetUniformLocation(this->field_0x4, "u0");
    this->field_0x34 = glGetUniformLocation(this->field_0x4, "u1");
    this->field_0x38 = glGetUniformLocation(this->field_0x4, "u2");
    this->field_0x3c = glGetUniformLocation(this->field_0x4, "u3");
    this->field_0x40 = glGetUniformLocation(this->field_0x4, "u4");
    this->field_0x44 = glGetUniformLocation(this->field_0x4, "u5");
    this->field_0x48 = glGetUniformLocation(this->field_0x4, "u6");
    this->field_0x4c = glGetUniformLocation(this->field_0x4, "u7");
    this->field_0x50 = glGetUniformLocation(this->field_0x4, "u8");
    this->field_0x54 = glGetUniformLocation(this->field_0x4, "u9");
    this->field_0x58 = glGetUniformLocation(this->field_0x4, "u10");
    this->field_0x5c = glGetUniformLocation(this->field_0x4, "u11");
    this->field_0x64 = glGetUniformLocation(this->field_0x4, "u12");
    this->field_0x6c = glGetUniformLocation(this->field_0x4, "u13");
    this->field_0x60 = glGetUniformLocation(this->field_0x4, "u14");
    this->field_0x68 = glGetUniformLocation(this->field_0x4, "u15");
    this->field_0x70 = glGetUniformLocation(this->field_0x4, "u16");
    this->field_0x74 = glGetUniformLocation(this->field_0x4, "u17");
    this->field_0x78 = glGetUniformLocation(this->field_0x4, "u18");
    this->field_0x7c = glGetUniformLocation(this->field_0x4, "u19");
    this->field_0x80 = glGetUniformLocation(this->field_0x4, "u20");
    this->field_0x84 = glGetUniformLocation(this->field_0x4, "u21");
    this->field_0x88 = glGetUniformLocation(this->field_0x4, "u22");
    this->field_0x8c = glGetUniformLocation(this->field_0x4, "u23");
    this->field_0x90 = glGetUniformLocation(this->field_0x4, "u24");
    this->field_0x94 = glGetUniformLocation(this->field_0x4, "u25");

    glUseProgram(this->field_0x4);
    for (int i = 0; i != 2; i++) {
        int loc = (&this->field_0x4c)[i];   // field_0x4c, field_0x50
        if (loc >= 0)
            glUniform1i(loc, i);
    }
    glUniform1i(this->field_0x54, 7);
}

} // namespace AbyssEngine
