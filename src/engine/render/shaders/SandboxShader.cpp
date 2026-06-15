#include "gof2/engine/render/shaders/SandboxShader.h"
#include "gof2/engine/render/Mesh.h"
#include "gof2/platform/gl.h"

// SandboxShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char _ZTVN11AbyssEngine13SandboxShaderE[];

// Registry slot the constructor links itself into (engine-global, resolved at link).
extern "C" void *SandboxShader_registerSrc;
extern "C" void **SandboxShader_registerDst;

namespace AbyssEngine {

void SandboxShader::UpdateMeshData(Mesh *meshArg, Engine *engine)
{
    AbyssEngine::Mesh *mesh = meshArg;
    char *e = (char *)engine;

    glUniformMatrix4fv(this->uniformA, 1, 0, (float *)(e + 0x104));

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

void SandboxShader::Init(Engine *)
{
    this->program = this->ES2LoadProgram("SandboxShader.vsh", "SandboxShader.fsh");

    this->attrPosition = glGetAttribLocation(this->program, "a_position");
    this->attrNormal   = glGetAttribLocation(this->program, "a_normal");
    this->attrTangent  = glGetAttribLocation(this->program, "a_tangent");
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

void SandboxShader::SetInActive()
{
    glDisableVertexAttribArray(this->attrPosition);
    glDisableVertexAttribArray(this->attrNormal);
    glDisableVertexAttribArray(this->attrTangent);
    glDisableVertexAttribArray(this->attrBinormal);
    glDisableVertexAttribArray(this->attrTexCoord);
}

SandboxShader::SandboxShader()
{
    this->vtable = _ZTVN11AbyssEngine13SandboxShaderE + 8;
    // Register this shader into the global registry list.
    *SandboxShader_registerDst = SandboxShader_registerSrc;
    this->name.s = u"SandboxShader";
}

} // namespace AbyssEngine
