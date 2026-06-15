#include "gof2/game/core/BumpShaderParticle.h"
#include "gof2/platform/gl.h"

// Per-frame particle lighting globals (renderer module-static state).
extern float g_particleGlobalA;
extern float g_particleGlobalB;

// BumpShaderParticle's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char _ZTVN11AbyssEngine18BumpShaderParticleE[];

namespace AbyssEngine {

int BumpShaderParticle::ShaderIndex;

BumpShaderParticle::BumpShaderParticle()
{
    this->vtable = _ZTVN11AbyssEngine18BumpShaderParticleE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"BumpShaderParticle";
}

void BumpShaderParticle::Init(Engine *)
{
    this->program = this->ES2LoadProgram("BumpShaderParticle.vsh", "BumpShaderParticle.fsh");

    attribA0 = glGetAttribLocation(this->program, "a0");
    attribA1 = glGetAttribLocation(this->program, "a1");
    attribA2 = glGetAttribLocation(this->program, "a2");
    attribA3 = glGetAttribLocation(this->program, "a3");
    attribA4 = glGetAttribLocation(this->program, "a4");

    uniformU0 = glGetUniformLocation(this->program, "u0");
    uniformU1 = glGetUniformLocation(this->program, "u1");
    uniformU2 = glGetUniformLocation(this->program, "u2");
    uniformU3 = glGetUniformLocation(this->program, "u3");
    uniformU4 = glGetUniformLocation(this->program, "u4");
    uniformU5 = glGetUniformLocation(this->program, "u5");
    uniformU6 = glGetUniformLocation(this->program, "u6");
    uniformU7 = glGetUniformLocation(this->program, "u7");
    uniformU8 = glGetUniformLocation(this->program, "u8");
    uniformU9 = glGetUniformLocation(this->program, "u9");
    uniformU10 = glGetUniformLocation(this->program, "u10");
    uniformU11 = glGetUniformLocation(this->program, "u11");
    uniformU12 = glGetUniformLocation(this->program, "u12");
    uniformU13 = glGetUniformLocation(this->program, "u13");

    glUseProgram(this->program);
    // Samplers u5 (+0x48) and u6 (+0x4c) are consecutive int fields, indexed at runtime.
    int *samplers = &uniformU5;
    for (int i = 0; i != 2; i++) {
        int loc = samplers[i];
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

void BumpShaderParticle::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (uniformU1 >= 0)
        glUniformMatrix4fv(uniformU1, 1, 0, (const float *)((char *)engine + 0x104));
    if (uniformU2 >= 0)
        glUniformMatrix3fv(uniformU2, 1, 0, (const float *)((char *)engine + 0x204));
    if (uniformU12 >= 0)
        glUniform1f(uniformU12, g_particleGlobalA);
    if (uniformU13 >= 0)
        glUniform1f(uniformU13, g_particleGlobalB);

    if (this->dirty != 0) {
        glUniform3f(uniformU3, field_f32(engine, 0x330),
                    field_f32(engine, 0x334), field_f32(engine, 0x338));
        if (uniformU4 >= 0)
            glUniform3f(uniformU4, field_f32(engine, 0x34c),
                        field_f32(engine, 0x350), field_f32(engine, 0x354));
        if (uniformU7 >= 0)
            glUniform4fv(uniformU7, 1, (float *)((char *)engine + 0xd0));
        if (uniformU8 >= 0)
            glUniform3fv(uniformU8, 1, (float *)((char *)engine + 0x314));
        if (uniformU9 >= 0)
            glUniform3fv(uniformU9, 1, (float *)((char *)engine + 0x2fc));
        if (uniformU10 >= 0)
            glUniform3fv(uniformU10, 1, (float *)((char *)engine + 0x2e4));
        if (uniformU11 >= 0)
            glUniform1f(uniformU11, field_f32(engine, 0x2c8));
        this->dirty = 0;
    }

    if (attribA0 >= 0)
        glEnableVertexAttribArray(attribA0);
    if (attribA1 >= 0)
        glEnableVertexAttribArray(attribA1);
    if (attribA2 >= 0)
        glEnableVertexAttribArray(attribA2);
    if (attribA3 >= 0)
        glEnableVertexAttribArray(attribA3);
    if (attribA4 >= 0)
        glEnableVertexAttribArray(attribA4);
    if (uniformU0 >= 0)
        glEnableVertexAttribArray(uniformU0);

    if (field_u8(mesh, 0x5c) == 0) {
        if (attribA0 >= 0)
            glVertexAttribPointer(attribA0, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (attribA1 >= 0)
            glVertexAttribPointer(attribA1, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (attribA2 >= 0)
            glVertexAttribPointer(attribA2, 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        if (attribA3 >= 0)
            glVertexAttribPointer(attribA3, 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        if (attribA4 >= 0)
            glVertexAttribPointer(attribA4, 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
        if (uniformU0 >= 0)
            glVertexAttribPointer(uniformU0, 4, 0x1406, 0, 0, field_ptr(mesh, 0xc));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(attribA0, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(attribA1, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(attribA2, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x70));
        glVertexAttribPointer(attribA3, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x74));
        glVertexAttribPointer(attribA4, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x78));
        glVertexAttribPointer(uniformU0, 4, 0x1406, 0, 0, 0);
    }
}

void BumpShaderParticle::SetInActive()
{
    if (attribA0 >= 0)
        glDisableVertexAttribArray(attribA0);
    if (attribA1 >= 0)
        glDisableVertexAttribArray(attribA1);
    if (attribA2 >= 0)
        glDisableVertexAttribArray(attribA2);
    if (attribA3 >= 0)
        glDisableVertexAttribArray(attribA3);
    if (attribA4 >= 0)
        glDisableVertexAttribArray(attribA4);
    if (uniformU0 >= 0)
        glDisableVertexAttribArray(uniformU0);
}

} // namespace AbyssEngine
