#include "BumpMapping.h"


extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(void *self);
extern "C" void _ZN11AbyssEngine6StringC2EPKcb(AbyssEngine::String *self, const char *text, bool copy);
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringaSERKS0_(AbyssEngine::String *self, const AbyssEngine::String *other);
extern "C" void _ZN11AbyssEngine6StringD2Ev(AbyssEngine::String *self);

// ---- _BumpMapping_914d0.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine11BumpMappingD0Ev(
    AbyssEngine::BumpMapping *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- Init_912f4.cpp ----
namespace AbyssEngine {

void BumpMapping::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpMapping.vsh", "BumpMapping.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_normal");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a_tangent");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a_binormal");
    field_i32(this, 0x30) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");

    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u_mvpMatrix");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u_lightDir");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u_texture");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u_normalMap");

    glUseProgram(field_i32(this, 0x04));
    glUniform1i(field_i32(this, 0x3c), 0);
    return glUniform1i(field_i32(this, 0x40), 1);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_913f8.cpp ----
namespace AbyssEngine {

void BumpMapping::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_u8(this, 0x9) != 0)
        field_u8(this, 0x9) = 0;

    glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x104);
    glUniform3f(field_i32(this, 0x38), field_f32(engine, 0x330), field_f32(engine, 0x334),
                field_f32(engine, 0x338));

    void (*enableVertexAttribArray)(uint32_t) = glEnableVertexAttribArray;
    enableVertexAttribArray(field_i32(this, 0x20));
    enableVertexAttribArray(field_i32(this, 0x24));
    enableVertexAttribArray(field_i32(this, 0x28));
    enableVertexAttribArray(field_i32(this, 0x2c));
    enableVertexAttribArray(field_i32(this, 0x30));

    glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    if ((field_u8(mesh, 0x0) & 2) != 0) {
        glVertexAttribPointer(field_i32(this, 0x30), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
    }
    if ((field_u8(mesh, 0x0) & 4) != 0) {
        void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
            glVertexAttribPointer;
        vertexAttribPointer(field_i32(this, 0x24), 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        vertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        vertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    }
}

} // namespace AbyssEngine

// ---- BumpMapping_91268.cpp ----
namespace AbyssEngine {

BumpMapping::BumpMapping()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev(this);
    *(uint32_t *)this = (uint32_t)_ZTVN11AbyssEngine11BumpMappingE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;

    char storage[sizeof(String)];
    String *name = (String *)storage;
    _ZN11AbyssEngine6StringC2EPKcb(name, "BumpMapping", false);
    _ZN11AbyssEngine6StringaSERKS0_((String *)((char *)this + 0xc), name);
    _ZN11AbyssEngine6StringD2Ev(name);
}

} // namespace AbyssEngine

// ---- SetInActive_913cc.cpp ----
namespace AbyssEngine {

void BumpMapping::SetInActive()
{
    void (*disableVertexAttribArray)(uint32_t) = glDisableVertexAttribArray;

    disableVertexAttribArray(field_i32(this, 0x20));
    disableVertexAttribArray(field_i32(this, 0x24));
    disableVertexAttribArray(field_i32(this, 0x28));
    disableVertexAttribArray(field_i32(this, 0x2c));
    disableVertexAttribArray(field_i32(this, 0x30));
}

} // namespace AbyssEngine
