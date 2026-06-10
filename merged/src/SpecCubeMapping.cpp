#include "SpecCubeMapping.h"


extern "C" void glUniform3fv(int location, int count, const float *value);
extern "C" void glUniformMatrix3fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glUniform4f(int location, float x, float y, float z, float w);

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
    field_i32(this, 0x04) = program;
    if (program == 0) {
        program = ((ShaderBaseStruct *)this)->ES2LoadProgram("SpecCubeMapping.vsh", "SpecCubeMapping.fsh");
        field_i32(this, 0x04) = program;
    }

    field_i32(this, 0x20) = glGetAttribLocation(program, "a0");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a1");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a2");

    field_i32(this, 0x2c) = glGetUniformLocation(field_i32(this, 0x04), "u0");
    field_i32(this, 0x30) = glGetUniformLocation(field_i32(this, 0x04), "u1");
    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u2");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u3");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u5");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u6");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u7");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u8");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u9");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u10");
    field_i32(this, 0x58) = glGetUniformLocation(field_i32(this, 0x04), "u11");

    glUseProgram(field_i32(this, 0x04));
    glUniform1i(field_i32(this, 0x40), 0);
    return glUniform1i(field_i32(this, 0x3c), 1);
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

    String name("SpecCubeMapping", false);
    *(String *)((char *)this + 0xc) = name;
}

} // namespace AbyssEngine

// ---- UpdateMeshData_929e4.cpp ----
// Additional GL entry points used by this shader's per-mesh upload.

namespace AbyssEngine {

// AbyssEngine::SpecCubeMapping::UpdateMeshData(Mesh*, Engine*)
//   Streams the per-frame uniforms (lighting cube data only once, then the
//   matrices/material) and binds the mesh's vertex attribute arrays, handling
//   both client-array and VBO meshes.
void SpecCubeMapping::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *eng = (char *)engine;
    char *m = (char *)mesh;

    if (field_u8(this, 0x9) != 0) {
        glUniform4fv(field_i32(this, 0x58), 1, (const float *)(eng + 0xd0));
        glUniform3fv(field_i32(this, 0x48), 1, (const float *)(eng + 0x2cc));
        glUniform3fv(field_i32(this, 0x4c), 1, (const float *)(eng + 0x2fc));
        glUniform3fv(field_i32(this, 0x50), 1, (const float *)(eng + 0x2e4));
        glUniform1f(field_i32(this, 0x54), *(float *)(eng + 0x2c8));
        field_u8(this, 0x9) = 0;
    }

    glUniform1f(field_i32(this, 0x44), *(float *)(eng + 0xcc));
    glUniformMatrix4fv(field_i32(this, 0x2c), 1, 0, eng + 0x104);
    glUniformMatrix3fv(field_i32(this, 0x30), 1, 0, eng + 0x204);
    glUniform4f(field_i32(this, 0x38),
                *(float *)(eng + 0x330), *(float *)(eng + 0x334),
                *(float *)(eng + 0x338), *(float *)(eng + 0x378));
    glUniform3f(field_i32(this, 0x34),
                *(float *)(eng + 0x34c), *(float *)(eng + 0x350), *(float *)(eng + 0x354));

    glEnableVertexAttribArray(field_i32(this, 0x20));
    glEnableVertexAttribArray(field_i32(this, 0x28));
    glEnableVertexAttribArray(field_i32(this, 0x24));

    if (field_u8(mesh, 0x5c) == 0) {
        // Client-side vertex arrays.
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, *(void **)(m + 4));
        uint8_t flags = field_u8(mesh, 0x0);
        if (flags & 2) {
            glVertexAttribPointer(field_i32(this, 0x28), 2, 0x1406, 0, 0, *(void **)(m + 8));
            flags = field_u8(mesh, 0x0);
        }
        if (((uint32_t)flags << 0x1d) & 0x80000000u) {
            glVertexAttribPointer(field_i32(this, 0x24), 3, 0x1406, 0, 0, *(void **)(m + 0x10));
        }
    } else {
        // Buffer-object meshes: bind each VBO before wiring up the attribute.
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(field_i32(this, 0x28), 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(field_i32(this, 0x24), 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- SetInActive_929c0.cpp ----
namespace AbyssEngine {

void SpecCubeMapping::SetInActive()
{
    void (*disableVertexAttribArray)(uint32_t) = glDisableVertexAttribArray;

    disableVertexAttribArray(field_i32(this, 0x20));
    disableVertexAttribArray(field_i32(this, 0x24));
    disableVertexAttribArray(field_i32(this, 0x28));
}

} // namespace AbyssEngine
