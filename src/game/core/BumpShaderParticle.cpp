#include "gof2/game/core/BumpShaderParticle.h"
#include "gof2/externs.h"

extern "C" void glUniform3fv(int location, int count, const float *value);

namespace AbyssEngine {

void BumpShaderParticle::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x38) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x38), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x3c) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x3c), 1, 0, (char *)engine + 0x204);
    if (field_i32(this, 0x64) >= 0)
        glUniform1f(field_i32(this, 0x64), g_particleGlobalA);
    if (field_i32(this, 0x68) >= 0)
        glUniform1f(field_i32(this, 0x68), g_particleGlobalB);

    if (field_u8(this, 0x9) != 0) {
        glUniform3f(field_i32(this, 0x40), field_f32(engine, 0x330),
                    field_f32(engine, 0x334), field_f32(engine, 0x338));
        if (field_i32(this, 0x44) >= 0)
            glUniform3f(field_i32(this, 0x44), field_f32(engine, 0x34c),
                        field_f32(engine, 0x350), field_f32(engine, 0x354));
        if (field_i32(this, 0x50) >= 0)
            glUniform4fv(field_i32(this, 0x50), 1, (float *)((char *)engine + 0xd0));
        if (field_i32(this, 0x54) >= 0)
            glUniform3fv(field_i32(this, 0x54), 1, (float *)((char *)engine + 0x314));
        if (field_i32(this, 0x58) >= 0)
            glUniform3fv(field_i32(this, 0x58), 1, (float *)((char *)engine + 0x2fc));
        if (field_i32(this, 0x5c) >= 0)
            glUniform3fv(field_i32(this, 0x5c), 1, (float *)((char *)engine + 0x2e4));
        if (field_i32(this, 0x60) >= 0)
            glUniform1f(field_i32(this, 0x60), field_f32(engine, 0x2c8));
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
    if (field_i32(this, 0x34) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x34));

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
        if (field_i32(this, 0x34) >= 0)
            glVertexAttribPointer(field_i32(this, 0x34), 4, 0x1406, 0, 0, field_ptr(mesh, 0xc));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x70));
        glVertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x74));
        glVertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x78));
        glVertexAttribPointer(field_i32(this, 0x34), 4, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

namespace AbyssEngine {

void BumpShaderParticle::SetInActive()
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
    loc = field_i32(this, 0x34);
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
        // name = String("BumpShaderParticle", false); this->name(0xc) = name; ~name.
        String tmp("BumpShaderParticle");
        ((String *)((char *)this + 0xc))->assign(&tmp);
    }
    
}

} // namespace AbyssEngine

namespace AbyssEngine {

void BumpShaderParticle::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpShaderParticle.vsh", "BumpShaderParticle.fsh");
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
    field_i32(this, 0x5c) = glGetUniformLocation(field_i32(this, 0x04), "u10");
    field_i32(this, 0x60) = glGetUniformLocation(field_i32(this, 0x04), "u11");
    field_i32(this, 0x64) = glGetUniformLocation(field_i32(this, 0x04), "u12");
    field_i32(this, 0x68) = glGetUniformLocation(field_i32(this, 0x04), "u13");

    glUseProgram(field_i32(this, 0x04));
    for (int i = 0; i != 2; i++) {
        int loc = field_i32(this, i * 4 + 0x48);
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

} // namespace AbyssEngine
