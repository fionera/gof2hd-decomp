#include "gof2/SpecCubeMapping.h"
#include "gof2/Mesh.h"


// ---- _SpecCubeMapping_92b60.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

void _ZN11AbyssEngine15SpecCubeMappingD0Ev(
    AbyssEngine::SpecCubeMapping *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- Init_9286c.cpp ----
namespace AbyssEngine {

void SpecCubeMapping::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->LoadBindShader("SpecCubeMapping.vsh", "SpecCubeMapping.fsh");
    this->programHandle = program;
    if (program == 0) {
        program = ((ShaderBaseStruct *)this)->ES2LoadProgram("SpecCubeMapping.vsh", "SpecCubeMapping.fsh");
        this->programHandle = program;
    }

    this->attribPosition = glGetAttribLocation(program, "a0");
    this->attribNormal = glGetAttribLocation(this->programHandle, "a1");
    this->attribTexCoord = glGetAttribLocation(this->programHandle, "a2");

    this->mvpMatrixLoc = glGetUniformLocation(this->programHandle, "u0");
    this->normalMatrixLoc = glGetUniformLocation(this->programHandle, "u1");
    this->field_0x34 = glGetUniformLocation(this->programHandle, "u2");
    this->field_0x38 = glGetUniformLocation(this->programHandle, "u3");
    this->samplerLoc0 = glGetUniformLocation(this->programHandle, "u4");
    this->samplerLoc1 = glGetUniformLocation(this->programHandle, "u5");
    this->field_0x44 = glGetUniformLocation(this->programHandle, "u6");
    this->field_0x48 = glGetUniformLocation(this->programHandle, "u7");
    this->field_0x4c = glGetUniformLocation(this->programHandle, "u8");
    this->field_0x50 = glGetUniformLocation(this->programHandle, "u9");
    this->field_0x54 = glGetUniformLocation(this->programHandle, "u10");
    this->field_0x58 = glGetUniformLocation(this->programHandle, "u11");

    glUseProgram(this->programHandle);
    glUniform1i(this->samplerLoc0, 0);
    glUniform1i(this->samplerLoc1, 1);
}

} // namespace AbyssEngine

// ---- SpecCubeMapping_927e0.cpp ----
namespace AbyssEngine {

// AbyssEngine::SpecCubeMapping::SpecCubeMapping()
//   Runs the ShaderBaseStruct base ctor, installs the SpecCubeMapping vtable,
//   publishes its shader index, and stores the shader resource name.
SpecCubeMapping::SpecCubeMapping()
{
    new ((void *)this) ShaderBaseStruct();

    // Install vtable (the +8 skips the RTTI/offset-to-top header words).
    *(void **)this = (void *)(_ZTVN11AbyssEngine15SpecCubeMappingE + 8);

    // SpecCubeMapping::ShaderIndex = ShaderBaseStruct::shaderIndexIntern.
    SpecCubeMapping::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;

    String name;
    name.s = u"SpecCubeMapping";
    this->shaderName = name;
}

} // namespace AbyssEngine

// ---- UpdateMeshData_929e4.cpp ----
// AbyssEngine::SpecCubeMapping::UpdateMeshData(Mesh*, Engine*)
//   Streams the per-frame uniforms (lighting cube data only once, then the
//   matrices/material) and binds the mesh's vertex attribute arrays, handling
//   both client-array and VBO meshes.
namespace AbyssEngine {

void SpecCubeMapping::UpdateMeshData(Mesh *meshArg, Engine *engine)
{
    Mesh *mesh = (Mesh *)meshArg;
    char *eng = (char *)engine;

    if (this->lightingDirty != 0) {
        glUniform4fv(this->field_0x58, 1, (const float *)(eng + 0xd0));
        glUniform3fv(this->field_0x48, 1, (const float *)(eng + 0x2cc));
        glUniform3fv(this->field_0x4c, 1, (const float *)(eng + 0x2fc));
        glUniform3fv(this->field_0x50, 1, (const float *)(eng + 0x2e4));
        glUniform1f(this->field_0x54, *(float *)(eng + 0x2c8));
        this->lightingDirty = 0;
    }

    glUniform1f(this->field_0x44, *(float *)(eng + 0xcc));
    glUniformMatrix4fv(this->mvpMatrixLoc, 1, 0, eng + 0x104);
    glUniformMatrix3fv(this->normalMatrixLoc, 1, 0, eng + 0x204);
    glUniform4f(this->field_0x38,
                *(float *)(eng + 0x330), *(float *)(eng + 0x334),
                *(float *)(eng + 0x338), *(float *)(eng + 0x378));
    glUniform3f(this->field_0x34,
                *(float *)(eng + 0x34c), *(float *)(eng + 0x350), *(float *)(eng + 0x354));

    glEnableVertexAttribArray(this->attribPosition);
    glEnableVertexAttribArray(this->attribTexCoord);
    glEnableVertexAttribArray(this->attribNormal);

    if (mesh->field_0x5c == 0) {
        // Client-side vertex arrays.
        glVertexAttribPointer(this->attribPosition, 3, 0x1406, 0, 0, mesh->field_0x4);
        uint8_t flags = mesh->field_0x0;
        if (flags & 2) {
            glVertexAttribPointer(this->attribTexCoord, 2, 0x1406, 0, 0, mesh->field_0x8);
            flags = mesh->field_0x0;
        }
        if (((uint32_t)flags << 0x1d) & 0x80000000u) {
            glVertexAttribPointer(this->attribNormal, 3, 0x1406, 0, 0, mesh->field_0x10);
        }
    } else {
        // Buffer-object meshes: bind each VBO before wiring up the attribute.
        glBindBuffer(0x8892, mesh->field_0x60);
        glVertexAttribPointer(this->attribPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x68);
        glVertexAttribPointer(this->attribTexCoord, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x6c);
        glVertexAttribPointer(this->attribNormal, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- SetInActive_929c0.cpp ----
namespace AbyssEngine {

void SpecCubeMapping::SetInActive()
{
    glDisableVertexAttribArray(this->attribPosition);
    glDisableVertexAttribArray(this->attribNormal);
    glDisableVertexAttribArray(this->attribTexCoord);
}

} // namespace AbyssEngine
