#include "gof2/BumpMapping.h"


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
    this->field_0x4 = program;

    this->field_0x20 = glGetAttribLocation(program, "a_position");
    this->field_0x24 = glGetAttribLocation(this->field_0x4, "a_normal");
    this->field_0x28 = glGetAttribLocation(this->field_0x4, "a_tangent");
    this->field_0x2c = glGetAttribLocation(this->field_0x4, "a_binormal");
    this->field_0x30 = glGetAttribLocation(this->field_0x4, "a_texCoord");

    this->field_0x34 = glGetUniformLocation(this->field_0x4, "u_mvpMatrix");
    this->field_0x38 = glGetUniformLocation(this->field_0x4, "u_lightDir");
    this->field_0x3c = glGetUniformLocation(this->field_0x4, "u_texture");
    this->field_0x40 = glGetUniformLocation(this->field_0x4, "u_normalMap");

    glUseProgram(this->field_0x4);
    glUniform1i(this->field_0x3c, 0);
    return glUniform1i(this->field_0x40, 1);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_913f8.cpp ----
namespace AbyssEngine {

void BumpMapping::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->field_0x9 != 0)
        this->field_0x9 = 0;

    glUniformMatrix4fv(this->field_0x34, 1, 0, (char *)engine + 0x104);
    glUniform3f(this->field_0x38, field_f32(engine, 0x330), field_f32(engine, 0x334),
                field_f32(engine, 0x338));

    void (*enableVertexAttribArray)(uint32_t) = glEnableVertexAttribArray;
    enableVertexAttribArray(this->field_0x20);
    enableVertexAttribArray(this->field_0x24);
    enableVertexAttribArray(this->field_0x28);
    enableVertexAttribArray(this->field_0x2c);
    enableVertexAttribArray(this->field_0x30);

    glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    if ((field_u8(mesh, 0x0) & 2) != 0) {
        glVertexAttribPointer(this->field_0x30, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
    }
    if ((field_u8(mesh, 0x0) & 4) != 0) {
        void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
            glVertexAttribPointer;
        vertexAttribPointer(this->field_0x24, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        vertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        vertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    }
}

} // namespace AbyssEngine

// ---- BumpMapping_91268.cpp ----
namespace AbyssEngine {

int BumpMapping::ShaderIndex;

BumpMapping::BumpMapping()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev(this);
    this->field_0x0 = _ZTVN11AbyssEngine11BumpMappingE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->field_0xc.s = u"BumpMapping";
}

} // namespace AbyssEngine

// ---- SetInActive_913cc.cpp ----
namespace AbyssEngine {

void BumpMapping::SetInActive()
{
    void (*disableVertexAttribArray)(uint32_t) = glDisableVertexAttribArray;

    disableVertexAttribArray(this->field_0x20);
    disableVertexAttribArray(this->field_0x24);
    disableVertexAttribArray(this->field_0x28);
    disableVertexAttribArray(this->field_0x2c);
    disableVertexAttribArray(this->field_0x30);
}

} // namespace AbyssEngine
