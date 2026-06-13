#include "gof2/game/core/BumpShaderV3.h"
#include "gof2/externs.h"
#include "gof2/engine/render/Mesh.h"

namespace AbyssEngine {

void BumpShaderV3::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpShaderV3.vsh", "BumpShaderV3.fsh");
    this->programId = program;

    this->attrib_position = glGetAttribLocation(program, "a0");
    this->attrib_texCoord = glGetAttribLocation(this->programId, "a1");
    this->attrib_normal = glGetAttribLocation(this->programId, "a2");
    this->attrib_tangent = glGetAttribLocation(this->programId, "a3");
    this->attrib_bitangent = glGetAttribLocation(this->programId, "a4");

    this->uniform_mvpMatrix = glGetUniformLocation(this->programId, "u0");
    this->uniform_normalMatrix = glGetUniformLocation(this->programId, "u1");
    this->field_0x3c = glGetUniformLocation(this->programId, "u2");
    this->field_0x40 = glGetUniformLocation(this->programId, "u3");
    this->field_0x44 = glGetUniformLocation(this->programId, "u4");
    this->uniform_texture0 = glGetUniformLocation(this->programId, "u5");
    this->uniform_texture1 = glGetUniformLocation(this->programId, "u6");
    this->uniform_texture2 = glGetUniformLocation(this->programId, "u7");
    this->field_0x54 = glGetUniformLocation(this->programId, "u8");
    this->field_0x58 = glGetUniformLocation(this->programId, "u9");
    this->field_0x60 = glGetUniformLocation(this->programId, "u10");
    this->field_0x68 = glGetUniformLocation(this->programId, "u11");
    this->field_0x5c = glGetUniformLocation(this->programId, "u12");
    this->field_0x64 = glGetUniformLocation(this->programId, "u13");
    this->field_0x6c = glGetUniformLocation(this->programId, "u14");
    this->field_0x70 = glGetUniformLocation(this->programId, "u15");
    this->field_0x74 = glGetUniformLocation(this->programId, "u16");
    this->field_0x78 = glGetUniformLocation(this->programId, "u17");
    this->field_0x7c = glGetUniformLocation(this->programId, "u18");
    this->field_0x80 = glGetUniformLocation(this->programId, "u19");

    glUseProgram(this->programId);
    // bind the three sampler uniforms (u5/u6/u7) to texture units 0/1/2.
    int textureUniforms[3] = { this->uniform_texture0, this->uniform_texture1,
                               this->uniform_texture2 };
    for (int i = 0; i != 3; i++) {
        int loc = textureUniforms[i];
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

} // namespace AbyssEngine

void _ZN11AbyssEngine12BumpShaderV3D0Ev(
    AbyssEngine::BumpShaderV3 *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

namespace AbyssEngine {

void BumpShaderV3::SetInActive()
{
    int loc;
    loc = this->attrib_position;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->attrib_texCoord;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->attrib_normal;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->attrib_tangent;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->attrib_bitangent;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

// Engine symbols used by the constructor (resolved at link time).
extern "C" {
// BumpShaderV3 vtable; the ctor stores (&vtable + 8) into this[0].
extern char _ZTVN11AbyssEngine12BumpShaderV3E[];
}

namespace AbyssEngine {

int BumpShaderV3::ShaderIndex;

// AbyssEngine::BumpShaderV3::BumpShaderV3()
BumpShaderV3::BumpShaderV3()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();

    // install vtable (+8 past the RTTI/offset slots) and copy the shader-index static.
    this->vtable = (void *)(_ZTVN11AbyssEngine12BumpShaderV3E + 8);
    ShaderBaseStruct::shaderIndexIntern = ShaderIndex;

    // name = String("BumpShaderV3"); this->name = name; ~name.
    String name("BumpShaderV3");
    this->name.assign(&name);
}

} // namespace AbyssEngine

namespace AbyssEngine {

void BumpShaderV3::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->uniform_mvpMatrix >= 0)
        glUniformMatrix4fv(this->uniform_mvpMatrix, 1, 0, (char *)engine + 0x104);
    if (this->uniform_normalMatrix >= 0)
        glUniformMatrix3fv(this->uniform_normalMatrix, 1, 0, (char *)engine + 0x204);
    if (this->field_0x78 >= 0)
        glUniform1f(this->field_0x78, *g_bsv3_floatA);
    if (this->field_0x7c >= 0)
        glUniform1f(this->field_0x7c, *g_bsv3_floatB);

    if (this->dirty != 0) {
        glUniform3f(this->field_0x3c, field_f32(engine, 0x330), field_f32(engine, 0x334),
                    field_f32(engine, 0x338));
        if (this->field_0x44 >= 0)
            glUniform3f(this->field_0x44, field_f32(engine, 0x34c), field_f32(engine, 0x350),
                        field_f32(engine, 0x354));
        if (this->field_0x54 >= 0)
            glUniform4fv(this->field_0x54, 1, (float *)((char *)engine + 0xd0));
        if (this->field_0x58 >= 0)
            glUniform3fv(this->field_0x58, 1, (float *)((char *)engine + 0x2cc));
        if (this->field_0x60 >= 0)
            glUniform3fv(this->field_0x60, 1, (float *)((char *)engine + 0x2fc));
        if (this->field_0x68 >= 0)
            glUniform3fv(this->field_0x68, 1, (float *)((char *)engine + 0x2e4));
        if (this->field_0x70 >= 0)
            glUniform1f(this->field_0x70, field_f32(engine, 0x2c8));
        if (this->field_0x74 >= 0)
            glUniform3fv(this->field_0x74, 1, (float *)((char *)engine + 0x320));
        if (this->field_0x80 >= 0) {
            float w = 0.85f;
            if (*(int *)((char *)((AbyssEngine::Mesh *)mesh)->field_0x30 + 0x24) != 0)
                w = 1.0f;
            glUniform1f(this->field_0x80, w);
        }
        if (field_i32(engine, 0x32c) >= 2) {
            void (*uniform3fv)(int, int, const float *) = glUniform3fv;
            uniform3fv(this->field_0x5c, 1, (float *)((char *)engine + 0x2d8));
            uniform3fv(this->field_0x64, 1, (float *)((char *)engine + 0x308));
            uniform3fv(this->field_0x6c, 1, (float *)((char *)engine + 0x2f0));
            glUniform3f(this->field_0x40, field_f32(engine, 0x33c), field_f32(engine, 0x340),
                        field_f32(engine, 0x344));
        } else {
            void (*uniform3f)(int, float, float, float) = glUniform3f;
            uniform3f(this->field_0x5c, 0, 0, 0);
            uniform3f(this->field_0x64, 0, 0, 0);
            uniform3f(this->field_0x6c, 0, 0, 0);
            uniform3f(this->field_0x40, field_f32(engine, 0x33c), field_f32(engine, 0x340),
                      field_f32(engine, 0x344));
        }
        this->dirty = 0;
    }

    if (this->attrib_position >= 0)
        glEnableVertexAttribArray(this->attrib_position);
    if (this->attrib_texCoord >= 0)
        glEnableVertexAttribArray(this->attrib_texCoord);
    if (this->attrib_normal >= 0)
        glEnableVertexAttribArray(this->attrib_normal);
    if (this->attrib_tangent >= 0)
        glEnableVertexAttribArray(this->attrib_tangent);
    if (this->attrib_bitangent >= 0)
        glEnableVertexAttribArray(this->attrib_bitangent);

    if (field_u8(mesh, 0x5c) == 0) {
        if (this->attrib_position >= 0)
            glVertexAttribPointer(this->attrib_position, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (this->attrib_texCoord >= 0)
            glVertexAttribPointer(this->attrib_texCoord, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (this->attrib_normal >= 0)
            glVertexAttribPointer(this->attrib_normal, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        if (this->attrib_tangent >= 0)
            glVertexAttribPointer(this->attrib_tangent, 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        if (this->attrib_bitangent >= 0)
            glVertexAttribPointer(this->attrib_bitangent, 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    } else {
        void (*bindBuffer)(uint32_t, uint32_t) = glBindBuffer;
        void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, field_i32(mesh, 0x60));
        vertexAttribPointer(this->attrib_position, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x68));
        vertexAttribPointer(this->attrib_texCoord, 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x6c));
        vertexAttribPointer(this->attrib_normal, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x70));
        vertexAttribPointer(this->attrib_tangent, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x74));
        vertexAttribPointer(this->attrib_bitangent, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
