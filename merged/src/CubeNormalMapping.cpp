#include "CubeNormalMapping.h"




// ---- UpdateMeshData_90cf0.cpp ----
namespace AbyssEngine {

void CubeNormalMapping::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_u8(this, 0x9) != 0) {
        void (*uniform4fv)(int, int, const float *) = glUniform4fv;
        uniform4fv(field_i32(this, 0x5c), 1, (float *)((char *)engine + 0xd0));
        uniform4fv(field_i32(this, 0x54), 1, (float *)((char *)engine + 0x2cc));
        uniform4fv(field_i32(this, 0x58), 1, (float *)((char *)engine + 0x2fc));
        uniform4fv(field_i32(this, 0x60), 1, (float *)((char *)engine + 0x2e4));
        glUniform1f(field_i32(this, 0x64), field_f32(engine, 0x2c8));
        field_u8(this, 0x9) = 0;
    }

    glUniform1f(field_i32(this, 0x50), field_f32(engine, 0xcc));
    glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x104);
    glUniformMatrix3fv(field_i32(this, 0x38), 1, 0, (char *)engine + 0x204);
    glUniform3f(field_i32(this, 0x40), field_f32(engine, 0x330), field_f32(engine, 0x334),
                field_f32(engine, 0x338));
    glUniform3f(field_i32(this, 0x3c), field_f32(engine, 0x34c), field_f32(engine, 0x350),
                field_f32(engine, 0x354));

    void (*enableVertexAttribArray)(uint32_t) = glEnableVertexAttribArray;
    enableVertexAttribArray(field_i32(this, 0x20));
    enableVertexAttribArray(field_i32(this, 0x28));
    enableVertexAttribArray(field_i32(this, 0x24));
    enableVertexAttribArray(field_i32(this, 0x2c));
    enableVertexAttribArray(field_i32(this, 0x30));

    if (field_u8(mesh, 0x5c) == 0) {
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if ((field_u8(mesh, 0x0) & 2) != 0)
            glVertexAttribPointer(field_i32(this, 0x28), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if ((field_u8(mesh, 0x0) & 4) != 0)
            glVertexAttribPointer(field_i32(this, 0x24), 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        if (field_i32(this, 0x2c) >= 0)
            glVertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        if (field_i32(this, 0x30) >= 0)
            glVertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    } else {
        void (*bindBuffer)(uint32_t, uint32_t) = glBindBuffer;
        void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, field_i32(mesh, 0x60));
        vertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x68));
        vertexAttribPointer(field_i32(this, 0x28), 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x6c));
        vertexAttribPointer(field_i32(this, 0x24), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x70));
        vertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x74));
        vertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, 0);
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
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u5");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u6");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u7");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u8");
    field_i32(this, 0x58) = glGetUniformLocation(field_i32(this, 0x04), "u9");
    field_i32(this, 0x60) = glGetUniformLocation(field_i32(this, 0x04), "u10");
    field_i32(this, 0x64) = glGetUniformLocation(field_i32(this, 0x04), "u11");
    field_i32(this, 0x5c) = glGetUniformLocation(field_i32(this, 0x04), "u12");

    glUseProgram(field_i32(this, 0x04));
    void (*uniform1i)(int, int) = glUniform1i;
    uniform1i(field_i32(this, 0x48), 0);
    uniform1i(field_i32(this, 0x4c), 1);
    uniform1i(field_i32(this, 0x44), 2);
}

} // namespace AbyssEngine

// ---- CubeNormalMapping_90ac0.cpp ----
extern "C" {
void _ZN11AbyssEngine16ShaderBaseStructC2Ev(AbyssEngine::ShaderBaseStruct *self);
extern void *_ZTIN11AbyssEngine17CubeNormalMappingE;
extern void *CubeNormalMapping_typeInfoSlot;
extern const char CubeNormalMapping_name[];
}

namespace AbyssEngine {

CubeNormalMapping::CubeNormalMapping()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);
    *(int *)this = (int)(_ZTVN11AbyssEngine17CubeNormalMappingE) + 8;
    CubeNormalMapping_typeInfoSlot = &_ZTIN11AbyssEngine17CubeNormalMappingE;
    String tmp(CubeNormalMapping_name, false);
    *(String *)((char *)this + 0xc) = tmp;
}

} // namespace AbyssEngine

// ---- SetInActive_90cc4.cpp ----
namespace AbyssEngine {

void CubeNormalMapping::SetInActive()
{
    void (*disableVertexAttribArray)(uint32_t) = glDisableVertexAttribArray;

    disableVertexAttribArray(field_i32(this, 0x20));
    disableVertexAttribArray(field_i32(this, 0x24));
    disableVertexAttribArray(field_i32(this, 0x28));
    disableVertexAttribArray(field_i32(this, 0x2c));
    disableVertexAttribArray(field_i32(this, 0x30));
}

} // namespace AbyssEngine
