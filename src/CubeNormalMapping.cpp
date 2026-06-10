#include "gof2/CubeNormalMapping.h"
#include "gof2/Mesh.h"



// ---- UpdateMeshData_90cf0.cpp ----
namespace AbyssEngine {

void CubeNormalMapping::UpdateMeshData(Mesh *meshArg, Engine *engine)
{
    AbyssEngine::Mesh *mesh = (AbyssEngine::Mesh *)meshArg;
    char *e = (char *)engine;
    if (this->field_0x9 != 0) {
        glUniform4fv(this->field_0x5c, 1, (float *)(e + 0xd0));
        glUniform4fv(this->field_0x54, 1, (float *)(e + 0x2cc));
        glUniform4fv(this->field_0x58, 1, (float *)(e + 0x2fc));
        glUniform4fv(this->field_0x60, 1, (float *)(e + 0x2e4));
        glUniform1f(this->field_0x64, *(float *)(e + 0x2c8));
        this->field_0x9 = 0;
    }

    glUniform1f(this->field_0x50, *(float *)(e + 0xcc));
    glUniformMatrix4fv(this->field_0x34, 1, 0, e + 0x104);
    glUniformMatrix3fv(this->field_0x38, 1, 0, e + 0x204);
    glUniform3f(this->field_0x40, *(float *)(e + 0x330), *(float *)(e + 0x334),
                *(float *)(e + 0x338));
    glUniform3f(this->field_0x3c, *(float *)(e + 0x34c), *(float *)(e + 0x350),
                *(float *)(e + 0x354));

    glEnableVertexAttribArray(this->field_0x20);
    glEnableVertexAttribArray(this->field_0x28);
    glEnableVertexAttribArray(this->field_0x24);
    glEnableVertexAttribArray(this->field_0x2c);
    glEnableVertexAttribArray(this->field_0x30);

    if (mesh->field_0x5c == 0) {
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, mesh->field_0x4);
        if ((mesh->field_0x0 & 2) != 0)
            glVertexAttribPointer(this->field_0x28, 2, 0x1406, 0, 0, mesh->field_0x8);
        if ((mesh->field_0x0 & 4) != 0)
            glVertexAttribPointer(this->field_0x24, 3, 0x1406, 0, 0, mesh->field_0x10);
        if (this->field_0x2c >= 0)
            glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, *(void **)&mesh->field_0x14);
        if (this->field_0x30 >= 0)
            glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, *(void **)&mesh->field_0x18);
    } else {
        glBindBuffer(0x8892, mesh->field_0x60);
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x68);
        glVertexAttribPointer(this->field_0x28, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x6c);
        glVertexAttribPointer(this->field_0x24, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x70);
        glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x74);
        glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- _CubeNormalMapping_90ed8.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine17CubeNormalMappingD0Ev(
    AbyssEngine::CubeNormalMapping *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- Init_90b4c.cpp ----
namespace AbyssEngine {

void CubeNormalMapping::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("CubeNormalMapping.vsh", "CubeNormalMapping.fsh");
    this->field_0x4 = program;

    this->field_0x20 = glGetAttribLocation(program, "a0");
    this->field_0x24 = glGetAttribLocation(this->field_0x4, "a1");
    this->field_0x28 = glGetAttribLocation(this->field_0x4, "a2");
    this->field_0x2c = glGetAttribLocation(this->field_0x4, "a3");
    this->field_0x30 = glGetAttribLocation(this->field_0x4, "a4");

    this->field_0x34 = glGetUniformLocation(this->field_0x4, "u0");
    this->field_0x38 = glGetUniformLocation(this->field_0x4, "u1");
    this->field_0x3c = glGetUniformLocation(this->field_0x4, "u2");
    this->field_0x40 = glGetUniformLocation(this->field_0x4, "u3");
    this->field_0x48 = glGetUniformLocation(this->field_0x4, "u4");
    this->field_0x44 = glGetUniformLocation(this->field_0x4, "u5");
    this->field_0x4c = glGetUniformLocation(this->field_0x4, "u6");
    this->field_0x50 = glGetUniformLocation(this->field_0x4, "u7");
    this->field_0x54 = glGetUniformLocation(this->field_0x4, "u8");
    this->field_0x58 = glGetUniformLocation(this->field_0x4, "u9");
    this->field_0x60 = glGetUniformLocation(this->field_0x4, "u10");
    this->field_0x64 = glGetUniformLocation(this->field_0x4, "u11");
    this->field_0x5c = glGetUniformLocation(this->field_0x4, "u12");

    glUseProgram(this->field_0x4);
    glUniform1i(this->field_0x48, 0);
    glUniform1i(this->field_0x4c, 1);
    glUniform1i(this->field_0x44, 2);
}

} // namespace AbyssEngine

// ---- CubeNormalMapping_90ac0.cpp ----
extern "C" {
void _ZN11AbyssEngine16ShaderBaseStructC2Ev(AbyssEngine::ShaderBaseStruct *self);
extern void *_ZTIN11AbyssEngine17CubeNormalMappingE;
extern void *CubeNormalMapping_typeInfoSlot;
extern const char CubeNormalMapping_name[];
extern char _ZTVN11AbyssEngine17CubeNormalMappingE[];
}

namespace AbyssEngine {

CubeNormalMapping::CubeNormalMapping()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);
    *(void **)this = (void *)(_ZTVN11AbyssEngine17CubeNormalMappingE + 8);
    CubeNormalMapping_typeInfoSlot = &_ZTIN11AbyssEngine17CubeNormalMappingE;
    String tmp;
    tmp.s = u"CubeNormalMapping";
    this->field_0xc = tmp;
}

} // namespace AbyssEngine

// ---- SetInActive_90cc4.cpp ----
namespace AbyssEngine {

void CubeNormalMapping::SetInActive()
{
    glDisableVertexAttribArray(this->field_0x20);
    glDisableVertexAttribArray(this->field_0x24);
    glDisableVertexAttribArray(this->field_0x28);
    glDisableVertexAttribArray(this->field_0x2c);
    glDisableVertexAttribArray(this->field_0x30);
}

} // namespace AbyssEngine
