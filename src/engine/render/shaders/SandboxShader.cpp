#include "gof2/engine/render/shaders/SandboxShader.h"
#include "gof2/engine/render/Mesh.h"

void _ZN11AbyssEngine13SandboxShaderD0Ev(
    AbyssEngine::SandboxShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

namespace AbyssEngine {

void SandboxShader::UpdateMeshData(Mesh *meshArg, Engine *engine)
{
    AbyssEngine::Mesh *mesh = (AbyssEngine::Mesh *)meshArg;
    char *e = (char *)engine;

    glUniformMatrix4fv(this->uniformA, 1, 0, e + 0x104);

    if (this->dirty != 0) {
        glUniform4fv(this->uniformF, 1, (float *)(e + 0xd0));
        glUniform3f(this->uniformB, *(float *)(e + 0x330), *(float *)(e + 0x334),
                    *(float *)(e + 0x338));
        glUniform3f(this->uniformC, *(float *)(e + 0x34c), *(float *)(e + 0x350),
                    *(float *)(e + 0x354));
        this->dirty = 0;
    }

    glEnableVertexAttribArray(this->attrPosition);
    glEnableVertexAttribArray(this->attrNormal);
    glEnableVertexAttribArray(this->attrTangent);
    glEnableVertexAttribArray(this->attrBinormal);
    glEnableVertexAttribArray(this->attrTexCoord);

    if (mesh->field_0x5c != 0) {
        glBindBuffer(0x8892, mesh->field_0x60);
        glVertexAttribPointer(this->attrPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x68);
        glVertexAttribPointer(this->attrNormal, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x6c);
        glVertexAttribPointer(this->attrTangent, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x70);
        glVertexAttribPointer(this->attrBinormal, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x74);
        glVertexAttribPointer(this->attrTexCoord, 3, 0x1406, 0, 0, 0);
        return;
    }

    glVertexAttribPointer(this->attrPosition, 3, 0x1406, 0, 0, mesh->field_0x4);
    glVertexAttribPointer(this->attrNormal, 2, 0x1406, 0, 0, mesh->field_0x8);
    glVertexAttribPointer(this->attrTangent, 3, 0x1406, 0, 0, mesh->field_0x10);
    glVertexAttribPointer(this->attrBinormal, 3, 0x1406, 0, 0, *(void **)&mesh->field_0x14);
    glVertexAttribPointer(this->attrTexCoord, 3, 0x1406, 0, 0, *(void **)&mesh->field_0x18);
}

} // namespace AbyssEngine

namespace AbyssEngine {

void SandboxShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("SandboxShader.vsh", "SandboxShader.fsh");
    this->program = program;

    this->attrPosition = glGetAttribLocation(program, "a_position");
    this->attrNormal = glGetAttribLocation(this->program, "a_normal");
    this->attrTangent = glGetAttribLocation(this->program, "a_tangent");
    this->attrBinormal = glGetAttribLocation(this->program, "a_binormal");
    this->attrTexCoord = glGetAttribLocation(this->program, "a_texCoord");

    this->uniformA = glGetUniformLocation(this->program, "u_a");
    this->uniformB = glGetUniformLocation(this->program, "u_b");
    this->uniformC = glGetUniformLocation(this->program, "u_c");
    this->uniformD = glGetUniformLocation(this->program, "u_d");
    this->uniformE = glGetUniformLocation(this->program, "u_e");
    this->uniformF = glGetUniformLocation(this->program, "u_f");
    this->uniformG = glGetUniformLocation(this->program, "u_g");

    glUseProgram(this->program);
    glUniform1i(this->uniformD, 0);
    glUniform1i(this->uniformE, 1);
}

} // namespace AbyssEngine

namespace AbyssEngine {

void SandboxShader::SetInActive()
{
    glDisableVertexAttribArray(this->attrPosition);
    glDisableVertexAttribArray(this->attrNormal);
    glDisableVertexAttribArray(this->attrTangent);
    glDisableVertexAttribArray(this->attrBinormal);
    glDisableVertexAttribArray(this->attrTexCoord);
}

} // namespace AbyssEngine

extern "C" {
extern void *SandboxShader_registerSrc;
extern void **SandboxShader_registerDst;
}

namespace AbyssEngine {

// AbyssEngine::SandboxShader::SandboxShader()
SandboxShader::SandboxShader()
{
    // Base ShaderBaseStruct ctor.
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();

    // Install the SandboxShader vtable (object pointer points 8 bytes into the
    // vtable, past the RTTI / offset-to-top slots).
    *(void **)this = (void *)(_ZTVN11AbyssEngine13SandboxShaderE + 8);

    // Register this shader into the global registry list.
    *SandboxShader_registerDst = SandboxShader_registerSrc;

    // Initialise the String member at +0xc from a literal.
    String tmp;
    tmp.s = u"SandboxShader";
    this->name = tmp;
}

} // namespace AbyssEngine
