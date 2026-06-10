#include "gof2/SandboxShader.h"
#include "gof2/Mesh.h"


// ---- _SandboxShader_91ad0.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine13SandboxShaderD0Ev(
    AbyssEngine::SandboxShader *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- UpdateMeshData_9194c.cpp ----
namespace AbyssEngine {

void SandboxShader::UpdateMeshData(Mesh *meshArg, Engine *engine)
{
    ::Mesh *mesh = (::Mesh *)meshArg;
    char *e = (char *)engine;

    glUniformMatrix4fv(this->field_0x34, 1, 0, e + 0x104);

    if (this->field_0x9 != 0) {
        glUniform4fv(this->field_0x40, 1, (float *)(e + 0xd0));
        glUniform3f(this->field_0x38, *(float *)(e + 0x330), *(float *)(e + 0x334),
                    *(float *)(e + 0x338));
        glUniform3f(this->field_0x3c, *(float *)(e + 0x34c), *(float *)(e + 0x350),
                    *(float *)(e + 0x354));
        this->field_0x9 = 0;
    }

    glEnableVertexAttribArray(this->field_0x20);
    glEnableVertexAttribArray(this->field_0x24);
    glEnableVertexAttribArray(this->field_0x28);
    glEnableVertexAttribArray(this->field_0x2c);
    glEnableVertexAttribArray(this->field_0x30);

    if (mesh->field_0x5c != 0) {
        glBindBuffer(0x8892, mesh->field_0x60);
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x68);
        glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x6c);
        glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x70);
        glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x74);
        glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, 0);
        return;
    }

    glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, mesh->field_0x4);
    glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, mesh->field_0x8);
    glVertexAttribPointer(this->field_0x28, 3, 0x1406, 0, 0, mesh->field_0x10);
    glVertexAttribPointer(this->field_0x2c, 3, 0x1406, 0, 0, *(void **)&mesh->field_0x14);
    glVertexAttribPointer(this->field_0x30, 3, 0x1406, 0, 0, *(void **)&mesh->field_0x18);
}

} // namespace AbyssEngine

// ---- Init_91818.cpp ----
namespace AbyssEngine {

void SandboxShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("SandboxShader.vsh", "SandboxShader.fsh");
    this->field_0x4 = program;

    this->field_0x20 = glGetAttribLocation(program, "a_position");
    this->field_0x24 = glGetAttribLocation(this->field_0x4, "a_normal");
    this->field_0x28 = glGetAttribLocation(this->field_0x4, "a_tangent");
    this->field_0x2c = glGetAttribLocation(this->field_0x4, "a_binormal");
    this->field_0x30 = glGetAttribLocation(this->field_0x4, "a_texCoord");

    this->field_0x34 = glGetUniformLocation(this->field_0x4, "u_a");
    this->field_0x38 = glGetUniformLocation(this->field_0x4, "u_b");
    this->field_0x3c = glGetUniformLocation(this->field_0x4, "u_c");
    this->field_0x44 = glGetUniformLocation(this->field_0x4, "u_d");
    this->field_0x48 = glGetUniformLocation(this->field_0x4, "u_e");
    this->field_0x40 = glGetUniformLocation(this->field_0x4, "u_f");
    this->field_0x4c = glGetUniformLocation(this->field_0x4, "u_g");

    glUseProgram(this->field_0x4);
    glUniform1i(this->field_0x44, 0);
    glUniform1i(this->field_0x48, 1);
}

} // namespace AbyssEngine

// ---- SetInActive_91920.cpp ----
namespace AbyssEngine {

void SandboxShader::SetInActive()
{
    glDisableVertexAttribArray(this->field_0x20);
    glDisableVertexAttribArray(this->field_0x24);
    glDisableVertexAttribArray(this->field_0x28);
    glDisableVertexAttribArray(this->field_0x2c);
    glDisableVertexAttribArray(this->field_0x30);
}

} // namespace AbyssEngine

// ---- SandboxShader_9178c.cpp ----
extern "C" {
void _ZN11AbyssEngine16ShaderBaseStructC2Ev(AbyssEngine::ShaderBaseStruct *self);
extern void *SandboxShader_registerSrc;
extern void **SandboxShader_registerDst;
}

namespace AbyssEngine {

// AbyssEngine::SandboxShader::SandboxShader()
SandboxShader::SandboxShader()
{
    // Base ShaderBaseStruct ctor.
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);

    // Install the SandboxShader vtable (object pointer points 8 bytes into the
    // vtable, past the RTTI / offset-to-top slots).
    *(void **)this = (void *)(_ZTVN11AbyssEngine13SandboxShaderE + 8);

    // Register this shader into the global registry list.
    *SandboxShader_registerDst = SandboxShader_registerSrc;

    // Initialise the String member at +0xc from a literal.
    String tmp;
    tmp.s = u"SandboxShader";
    this->field_0xc = tmp;
}

} // namespace AbyssEngine
