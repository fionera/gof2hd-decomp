#include "gof2/engine/render/shaders/GenericShader2.h"
#include "gof2/platform/gl.h"

// GenericShader2's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char GenericShader2_vtable;

namespace AbyssEngine {

int GenericShader2::ShaderIndex;

// AbyssEngine::GenericShader2::SetInActive()
void GenericShader2::SetInActive()
{
    if (aPosition >= 0)
        glDisableVertexAttribArray(aPosition);
    if (aNormal >= 0)
        glDisableVertexAttribArray(aNormal);
    if (aTangent >= 0)
        glDisableVertexAttribArray(aTangent);
    if (aBinormal >= 0)
        glDisableVertexAttribArray(aBinormal);
    if (aTexCoord >= 0)
        glDisableVertexAttribArray(aTexCoord);
}

// AbyssEngine::GenericShader2::Init(AbyssEngine::Engine*)
void GenericShader2::Init(Engine *)
{
    this->program = this->ES2LoadProgram("GenericShader2.vsh", "GenericShader2.fsh");

    aPosition = glGetAttribLocation(this->program, "a_position");
    aNormal = glGetAttribLocation(this->program, "a_normal");
    aTangent = glGetAttribLocation(this->program, "a_tangent");
    aBinormal = glGetAttribLocation(this->program, "a_binormal");
    aTexCoord = glGetAttribLocation(this->program, "a_texCoord");

    uM0 = glGetUniformLocation(this->program, "u_m0");
    uM1 = glGetUniformLocation(this->program, "u_m1");
    uM2 = glGetUniformLocation(this->program, "u_m2");
    uM3 = glGetUniformLocation(this->program, "u_m3");
    uM4 = glGetUniformLocation(this->program, "u_m4");
    uM5 = glGetUniformLocation(this->program, "u_m5");
    uM6 = glGetUniformLocation(this->program, "u_m6");
    uM7 = glGetUniformLocation(this->program, "u_m7");
    uM8 = glGetUniformLocation(this->program, "u_m8");

    glUseProgram(this->program);
    glUniform1i(uM4, 0);
}

// AbyssEngine::GenericShader2::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void GenericShader2::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    if (uM0 >= 0)
        glUniformMatrix4fv(uM0, 1, 0, (const float *)(e + 0x104));
    if (uM1 >= 0)
        glUniformMatrix3fv(uM1, 1, 0, (const float *)(e + 0x204));

    if (this->dirty != 0) {
        if (uM2 >= 0)
            glUniform3f(uM2, *(float *)(e + 0x330), *(float *)(e + 0x334), *(float *)(e + 0x338));
        if (uM3 >= 0)
            glUniform3f(uM3, *(float *)(e + 0x34c), *(float *)(e + 0x350), *(float *)(e + 0x354));
        if (uM5 >= 0)
            glUniform4fv(uM5, 1, (const float *)(e + 0xd0));
        if (uM6 >= 0)
            glUniform4fv(uM6, 1, (const float *)(e + 0x2a8));
        if (uM7 >= 0)
            glUniform4fv(uM7, 1, (const float *)(e + 0x298));
        if (uM8 >= 0)
            glUniform4fv(uM8, 1, (const float *)(e + 0x2b8));
        this->dirty = 0;
    }

    if (aPosition >= 0)
        glEnableVertexAttribArray(aPosition);
    if (aNormal >= 0)
        glEnableVertexAttribArray(aNormal);
    if (aTangent >= 0)
        glEnableVertexAttribArray(aTangent);
    if (aBinormal >= 0)
        glEnableVertexAttribArray(aBinormal);
    if (aTexCoord >= 0)
        glEnableVertexAttribArray(aTexCoord);

    if (aPosition >= 0)
        glVertexAttribPointer(aPosition, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
    if (aNormal >= 0)
        glVertexAttribPointer(aNormal, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
    if (aTangent >= 0)
        glVertexAttribPointer(aTangent, 3, 0x1406, 0, 0, *(void **)(m + 0x10));
    if (aBinormal >= 0)
        glVertexAttribPointer(aBinormal, 3, 0x1406, 0, 0, *(void **)(m + 0x14));
    if (aTexCoord >= 0)
        glVertexAttribPointer(aTexCoord, 3, 0x1406, 0, 0, *(void **)(m + 0x18));
}

// AbyssEngine::GenericShader2::GenericShader2()
GenericShader2::GenericShader2()
{
    this->vtable = &GenericShader2_vtable + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"GenericShader2";
}

} // namespace AbyssEngine
