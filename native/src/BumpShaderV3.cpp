#include "gof2/BumpShaderV3.h"
#include "gof2/Mesh.h"


extern "C" float *g_bsv3_floatA;
extern "C" float *g_bsv3_floatB;

// ---- Init_8c1a8.cpp ----
namespace AbyssEngine {

void BumpShaderV3::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpShaderV3.vsh", "BumpShaderV3.fsh");
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
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u5");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u6");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u7");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u8");
    field_i32(this, 0x58) = glGetUniformLocation(field_i32(this, 0x04), "u9");
    field_i32(this, 0x60) = glGetUniformLocation(field_i32(this, 0x04), "u10");
    field_i32(this, 0x68) = glGetUniformLocation(field_i32(this, 0x04), "u11");
    field_i32(this, 0x5c) = glGetUniformLocation(field_i32(this, 0x04), "u12");
    field_i32(this, 0x64) = glGetUniformLocation(field_i32(this, 0x04), "u13");
    field_i32(this, 0x6c) = glGetUniformLocation(field_i32(this, 0x04), "u14");
    field_i32(this, 0x70) = glGetUniformLocation(field_i32(this, 0x04), "u15");
    field_i32(this, 0x74) = glGetUniformLocation(field_i32(this, 0x04), "u16");
    field_i32(this, 0x78) = glGetUniformLocation(field_i32(this, 0x04), "u17");
    field_i32(this, 0x7c) = glGetUniformLocation(field_i32(this, 0x04), "u18");
    field_i32(this, 0x80) = glGetUniformLocation(field_i32(this, 0x04), "u19");

    glUseProgram(field_i32(this, 0x04));
    for (int i = 0; i != 3; i++) {
        int loc = field_i32(this, i * 4 + 0x48);
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

} // namespace AbyssEngine

// ---- _BumpShaderV3_8c6b0.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine12BumpShaderV3D0Ev(
    AbyssEngine::BumpShaderV3 *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- SetInActive_8c38c.cpp ----
namespace AbyssEngine {

void BumpShaderV3::SetInActive()
{
    int loc;
    loc = field_i32(this, 0x20);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x24);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x28);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x2c);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = field_i32(this, 0x30);
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

// ---- BumpShaderV3_8c11c.cpp ----
// Engine symbols used by the constructor (resolved at link time).
extern "C" {
// AbyssEngine::ShaderBaseStruct::ShaderBaseStruct(this) -- 0x00070930.
void *_ZN11AbyssEngine16ShaderBaseStructC2Ev(void *self);
// AbyssEngine::String::String(char const*, bool) / operator= / ~String.
void _ZN11AbyssEngine6StringC2EPKcb(AbyssEngine::String *self, const char *text, bool copy);
AbyssEngine::String &_ZN11AbyssEngine6StringaSERKS0_(AbyssEngine::String *self,
                                                     const AbyssEngine::String *other);
void _ZN11AbyssEngine6StringD2Ev(AbyssEngine::String *self);

// BumpShaderV3 vtable; the ctor stores (&vtable + 8) into this[0].
extern char _ZTVN11AbyssEngine12BumpShaderV3E[];
// Two statics related by the engine: ShaderBaseStruct::shaderIndexIntern = BumpShaderV3::ShaderIndex.
extern int _ZN11AbyssEngine12BumpShaderV311ShaderIndexE;
extern int _ZN11AbyssEngine16ShaderBaseStruct16shaderIndexInternE;
}

namespace AbyssEngine {

// AbyssEngine::BumpShaderV3::BumpShaderV3()
BumpShaderV3::BumpShaderV3()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev(this);

    // install vtable (+8 past the RTTI/offset slots) and copy the shader-index static.
    *(void **)this = (void *)(_ZTVN11AbyssEngine12BumpShaderV3E + 8);
    _ZN11AbyssEngine16ShaderBaseStruct16shaderIndexInternE =
        _ZN11AbyssEngine12BumpShaderV311ShaderIndexE;

    // name = String("BumpShaderV3"); this->name(0xc) = name; ~name.
    String name;
    _ZN11AbyssEngine6StringC2EPKcb(&name, "BumpShaderV3", false);
    _ZN11AbyssEngine6StringaSERKS0_((String *)((char *)this + 0xc), &name);
    _ZN11AbyssEngine6StringD2Ev(&name);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8c3cc.cpp ----
namespace AbyssEngine {

void BumpShaderV3::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x34) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x38) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x38), 1, 0, (char *)engine + 0x204);
    if (field_i32(this, 0x78) >= 0)
        glUniform1f(field_i32(this, 0x78), *g_bsv3_floatA);
    if (field_i32(this, 0x7c) >= 0)
        glUniform1f(field_i32(this, 0x7c), *g_bsv3_floatB);

    if (field_u8(this, 0x9) != 0) {
        glUniform3f(field_i32(this, 0x3c), field_f32(engine, 0x330), field_f32(engine, 0x334),
                    field_f32(engine, 0x338));
        if (field_i32(this, 0x44) >= 0)
            glUniform3f(field_i32(this, 0x44), field_f32(engine, 0x34c), field_f32(engine, 0x350),
                        field_f32(engine, 0x354));
        if (field_i32(this, 0x54) >= 0)
            glUniform4fv(field_i32(this, 0x54), 1, (float *)((char *)engine + 0xd0));
        if (field_i32(this, 0x58) >= 0)
            glUniform3fv(field_i32(this, 0x58), 1, (float *)((char *)engine + 0x2cc));
        if (field_i32(this, 0x60) >= 0)
            glUniform3fv(field_i32(this, 0x60), 1, (float *)((char *)engine + 0x2fc));
        if (field_i32(this, 0x68) >= 0)
            glUniform3fv(field_i32(this, 0x68), 1, (float *)((char *)engine + 0x2e4));
        if (field_i32(this, 0x70) >= 0)
            glUniform1f(field_i32(this, 0x70), field_f32(engine, 0x2c8));
        if (field_i32(this, 0x74) >= 0)
            glUniform3fv(field_i32(this, 0x74), 1, (float *)((char *)engine + 0x320));
        if (field_i32(this, 0x80) >= 0) {
            float w = 0.85f;
            if (*(int *)((char *)((::Mesh *)mesh)->field_0x30 + 0x24) != 0)
                w = 1.0f;
            glUniform1f(field_i32(this, 0x80), w);
        }
        if (field_i32(engine, 0x32c) >= 2) {
            void (*uniform3fv)(int, int, const float *) = glUniform3fv;
            uniform3fv(field_i32(this, 0x5c), 1, (float *)((char *)engine + 0x2d8));
            uniform3fv(field_i32(this, 0x64), 1, (float *)((char *)engine + 0x308));
            uniform3fv(field_i32(this, 0x6c), 1, (float *)((char *)engine + 0x2f0));
            glUniform3f(field_i32(this, 0x40), field_f32(engine, 0x33c), field_f32(engine, 0x340),
                        field_f32(engine, 0x344));
        } else {
            void (*uniform3f)(int, float, float, float) = glUniform3f;
            uniform3f(field_i32(this, 0x5c), 0, 0, 0);
            uniform3f(field_i32(this, 0x64), 0, 0, 0);
            uniform3f(field_i32(this, 0x6c), 0, 0, 0);
            uniform3f(field_i32(this, 0x40), field_f32(engine, 0x33c), field_f32(engine, 0x340),
                      field_f32(engine, 0x344));
        }
        field_u8(this, 0x9) = 0;
    }

    if (field_i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x20));
    if (field_i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x24));
    if (field_i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x28));
    if (field_i32(this, 0x2c) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x2c));
    if (field_i32(this, 0x30) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x30));

    if (field_u8(mesh, 0x5c) == 0) {
        if (field_i32(this, 0x20) >= 0)
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (field_i32(this, 0x24) >= 0)
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (field_i32(this, 0x28) >= 0)
            glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
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
        vertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x6c));
        vertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x70));
        vertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x74));
        vertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
