#include "gof2/SpecCubeMapping.h"
#include "gof2/Mesh.h"


// ---- _SpecCubeMapping_92b60.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine15SpecCubeMappingD0Ev(
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
    this->field_0x4 = program;
    if (program == 0) {
        program = ((ShaderBaseStruct *)this)->ES2LoadProgram("SpecCubeMapping.vsh", "SpecCubeMapping.fsh");
        this->field_0x4 = program;
    }

    this->field_0x20 = glGetAttribLocation(program, "a0");
    this->field_0x24 = glGetAttribLocation(this->field_0x4, "a1");
    this->field_0x28 = glGetAttribLocation(this->field_0x4, "a2");

    this->field_0x2c = glGetUniformLocation(this->field_0x4, "u0");
    this->field_0x30 = glGetUniformLocation(this->field_0x4, "u1");
    this->field_0x34 = glGetUniformLocation(this->field_0x4, "u2");
    this->field_0x38 = glGetUniformLocation(this->field_0x4, "u3");
    this->field_0x40 = glGetUniformLocation(this->field_0x4, "u4");
    this->field_0x3c = glGetUniformLocation(this->field_0x4, "u5");
    this->field_0x44 = glGetUniformLocation(this->field_0x4, "u6");
    this->field_0x48 = glGetUniformLocation(this->field_0x4, "u7");
    this->field_0x4c = glGetUniformLocation(this->field_0x4, "u8");
    this->field_0x50 = glGetUniformLocation(this->field_0x4, "u9");
    this->field_0x54 = glGetUniformLocation(this->field_0x4, "u10");
    this->field_0x58 = glGetUniformLocation(this->field_0x4, "u11");

    glUseProgram(this->field_0x4);
    glUniform1i(this->field_0x40, 0);
    glUniform1i(this->field_0x3c, 1);
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
    this->field_0xc = name;
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

    if (this->field_0x9 != 0) {
        glUniform4fv(this->field_0x58, 1, (const float *)(eng + 0xd0));
        glUniform3fv(this->field_0x48, 1, (const float *)(eng + 0x2cc));
        glUniform3fv(this->field_0x4c, 1, (const float *)(eng + 0x2fc));
        glUniform3fv(this->field_0x50, 1, (const float *)(eng + 0x2e4));
        glUniform1f(this->field_0x54, *(float *)(eng + 0x2c8));
        this->field_0x9 = 0;
    }

    glUniform1f(this->field_0x44, *(float *)(eng + 0xcc));
    glUniformMatrix4fv(this->field_0x2c, 1, 0, eng + 0x104);
    glUniformMatrix3fv(this->field_0x30, 1, 0, eng + 0x204);
    glUniform4f(this->field_0x38,
                *(float *)(eng + 0x330), *(float *)(eng + 0x334),
                *(float *)(eng + 0x338), *(float *)(eng + 0x378));
    glUniform3f(this->field_0x34,
                *(float *)(eng + 0x34c), *(float *)(eng + 0x350), *(float *)(eng + 0x354));

    glEnableVertexAttribArray(this->field_0x20);
    glEnableVertexAttribArray(this->field_0x28);
    glEnableVertexAttribArray(this->field_0x24);

    if (mesh->field_0x5c == 0) {
        // Client-side vertex arrays.
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, mesh->field_0x4);
        uint8_t flags = mesh->field_0x0;
        if (flags & 2) {
            glVertexAttribPointer(this->field_0x28, 2, 0x1406, 0, 0, mesh->field_0x8);
            flags = mesh->field_0x0;
        }
        if (((uint32_t)flags << 0x1d) & 0x80000000u) {
            glVertexAttribPointer(this->field_0x24, 3, 0x1406, 0, 0, mesh->field_0x10);
        }
    } else {
        // Buffer-object meshes: bind each VBO before wiring up the attribute.
        glBindBuffer(0x8892, mesh->field_0x60);
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x68);
        glVertexAttribPointer(this->field_0x28, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x6c);
        glVertexAttribPointer(this->field_0x24, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- SetInActive_929c0.cpp ----
namespace AbyssEngine {

void SpecCubeMapping::SetInActive()
{
    glDisableVertexAttribArray(this->field_0x20);
    glDisableVertexAttribArray(this->field_0x24);
    glDisableVertexAttribArray(this->field_0x28);
}

} // namespace AbyssEngine
