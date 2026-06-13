#include "gof2/game/core/BumpShaderParticle.h"
#include "gof2/externs.h"

extern "C" void glUniform3fv(int location, int count, const float *value);

namespace AbyssEngine {

void BumpShaderParticle::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (uniformU1 >= 0)
        glUniformMatrix4fv(uniformU1, 1, 0, (char *)engine + 0x104);
    if (uniformU2 >= 0)
        glUniformMatrix3fv(uniformU2, 1, 0, (char *)engine + 0x204);
    if (uniformU12 >= 0)
        glUniform1f(uniformU12, g_particleGlobalA);
    if (uniformU13 >= 0)
        glUniform1f(uniformU13, g_particleGlobalB);

    if (field_0x9 != 0) {
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
        field_0x9 = 0;
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

} // namespace AbyssEngine

namespace AbyssEngine {

void BumpShaderParticle::SetInActive()
{
    int loc;
    loc = attribA0;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = attribA1;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = attribA2;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = attribA3;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = attribA4;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = uniformU0;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

void _ZN11AbyssEngine18BumpShaderParticleD0Ev(
    AbyssEngine::BumpShaderParticle *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

namespace AbyssEngine {

int BumpShaderParticle::ShaderIndex;

__attribute__((minsize)) BumpShaderParticle::BumpShaderParticle()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    *(void **)this = (void *)(_ZTVN11AbyssEngine18BumpShaderParticleE + 8);
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    {
        // name = String("BumpShaderParticle", false);
        String tmp("BumpShaderParticle");
        name.assign(&tmp);
    }

}

} // namespace AbyssEngine

namespace AbyssEngine {

void BumpShaderParticle::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpShaderParticle.vsh", "BumpShaderParticle.fsh");
    field_0x4 = program;

    attribA0 = glGetAttribLocation(program, "a0");
    attribA1 = glGetAttribLocation(field_0x4, "a1");
    attribA2 = glGetAttribLocation(field_0x4, "a2");
    attribA3 = glGetAttribLocation(field_0x4, "a3");
    attribA4 = glGetAttribLocation(field_0x4, "a4");

    uniformU0 = glGetUniformLocation(field_0x4, "u0");
    uniformU1 = glGetUniformLocation(field_0x4, "u1");
    uniformU2 = glGetUniformLocation(field_0x4, "u2");
    uniformU3 = glGetUniformLocation(field_0x4, "u3");
    uniformU4 = glGetUniformLocation(field_0x4, "u4");
    uniformU5 = glGetUniformLocation(field_0x4, "u5");
    uniformU6 = glGetUniformLocation(field_0x4, "u6");
    uniformU7 = glGetUniformLocation(field_0x4, "u7");
    uniformU8 = glGetUniformLocation(field_0x4, "u8");
    uniformU9 = glGetUniformLocation(field_0x4, "u9");
    uniformU10 = glGetUniformLocation(field_0x4, "u10");
    uniformU11 = glGetUniformLocation(field_0x4, "u11");
    uniformU12 = glGetUniformLocation(field_0x4, "u12");
    uniformU13 = glGetUniformLocation(field_0x4, "u13");

    glUseProgram(field_0x4);
    // samplers u5 (+0x48) and u6 (+0x4c): consecutive int fields, indexed at runtime.
    int *samplers = &uniformU5;
    for (int i = 0; i != 2; i++) {
        int loc = samplers[i];
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

} // namespace AbyssEngine
