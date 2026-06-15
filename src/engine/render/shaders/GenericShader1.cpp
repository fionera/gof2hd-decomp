#include "gof2/engine/render/shaders/GenericShader1.h"
#include "gof2/platform/gl.h"

// GenericShader1's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char GenericShader1_vtable;

namespace AbyssEngine {

GenericShader1::GenericShader1()
{
    this->vtable = &GenericShader1_vtable + 8;
    this->name.s = u"GenericShader1";
}

// Compiles GenericShader1.vsh/.fsh and caches the attribute and u_m0..u_m8 uniform locations.
void GenericShader1::Init(Engine *)
{
    this->program = this->ES2LoadProgram("GenericShader1.vsh", "GenericShader1.fsh");

    this->aPosition = glGetAttribLocation(this->program, "a_position");
    this->aNormal = glGetAttribLocation(this->program, "a_normal");
    this->aTangent = glGetAttribLocation(this->program, "a_tangent");
    this->aBinormal = glGetAttribLocation(this->program, "a_binormal");
    this->aTexCoord = glGetAttribLocation(this->program, "a_texCoord");

    this->uM0 = glGetUniformLocation(this->program, "u_m0");
    this->uM1 = glGetUniformLocation(this->program, "u_m1");
    this->uM2 = glGetUniformLocation(this->program, "u_m2");
    this->uM3 = glGetUniformLocation(this->program, "u_m3");
    this->uM4 = glGetUniformLocation(this->program, "u_m4");
    this->uM5 = glGetUniformLocation(this->program, "u_m5");
    this->uM6 = glGetUniformLocation(this->program, "u_m6");
    this->uM7 = glGetUniformLocation(this->program, "u_m7");
    this->uM8 = glGetUniformLocation(this->program, "u_m8");

    glUseProgram(this->program);
    glUniform1i(this->uM4, 0);
}

// Pushes the mesh's transforms and (when material data is dirty) lighting/material uniforms,
// then binds the vertex attribute arrays.
void GenericShader1::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    if (this->uM0 >= 0)
        glUniformMatrix4fv(this->uM0, 1, 0, (const float *)(e + 0x104));
    if (this->uM1 >= 0)
        glUniformMatrix3fv(this->uM1, 1, 0, (const float *)(e + 0x204));

    if (this->dirty != 0) {
        if (this->uM2 >= 0)
            glUniform3f(this->uM2, *(float *)(e + 0x330), *(float *)(e + 0x334),
                        *(float *)(e + 0x338));
        if (this->uM3 >= 0)
            glUniform3f(this->uM3, *(float *)(e + 0x34c), *(float *)(e + 0x350),
                        *(float *)(e + 0x354));
        if (this->uM5 >= 0)
            glUniform4fv(this->uM5, 1, (const float *)(e + 0xd0));
        if (this->uM6 >= 0)
            glUniform4fv(this->uM6, 1, (const float *)(e + 0x2a8));
        if (this->uM7 >= 0)
            glUniform4fv(this->uM7, 1, (const float *)(e + 0x298));
        if (this->uM8 >= 0)
            glUniform4fv(this->uM8, 1, (const float *)(e + 0x2b8));
        this->dirty = 0;
    }

    if (this->aPosition >= 0)
        glEnableVertexAttribArray(this->aPosition);
    if (this->aNormal >= 0)
        glEnableVertexAttribArray(this->aNormal);
    if (this->aTangent >= 0)
        glEnableVertexAttribArray(this->aTangent);
    if (this->aBinormal >= 0)
        glEnableVertexAttribArray(this->aBinormal);
    if (this->aTexCoord >= 0)
        glEnableVertexAttribArray(this->aTexCoord);

    if (this->aPosition >= 0)
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
    if (this->aNormal >= 0)
        glVertexAttribPointer(this->aNormal, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
    if (this->aTangent >= 0)
        glVertexAttribPointer(this->aTangent, 3, 0x1406, 0, 0, *(void **)(m + 0x10));
    if (this->aBinormal >= 0)
        glVertexAttribPointer(this->aBinormal, 3, 0x1406, 0, 0, *(void **)(m + 0x14));
    if (this->aTexCoord >= 0)
        glVertexAttribPointer(this->aTexCoord, 3, 0x1406, 0, 0, *(void **)(m + 0x18));
}

// Disables the vertex attribute arrays this shader enabled.
void GenericShader1::SetInActive()
{
    if (this->aPosition >= 0)
        glDisableVertexAttribArray(this->aPosition);
    if (this->aNormal >= 0)
        glDisableVertexAttribArray(this->aNormal);
    if (this->aTangent >= 0)
        glDisableVertexAttribArray(this->aTangent);
    if (this->aBinormal >= 0)
        glDisableVertexAttribArray(this->aBinormal);
    if (this->aTexCoord >= 0)
        glDisableVertexAttribArray(this->aTexCoord);
}

} // namespace AbyssEngine
