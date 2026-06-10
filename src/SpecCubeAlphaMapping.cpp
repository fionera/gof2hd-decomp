#include "gof2/SpecCubeAlphaMapping.h"
#include "gof2/String.h"


// ---- Init_87a10.cpp ----
namespace AbyssEngine {

void SpecCubeAlphaMapping::Init(Engine *)
{
    int program = ShaderBaseStruct_LoadBindShader(this, "SpecCubeAlphaMapping.vsh", "SpecCubeAlphaMapping.fsh");
    this->field_0x4 = program;
    if (program == 0) {
        program = ShaderBaseStruct_ES2LoadProgram(this, "SpecCubeAlphaMapping.vsh", "SpecCubeAlphaMapping2.fsh");
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
    return glUniform1i(this->field_0x3c, 1);
}

} // namespace AbyssEngine

// ---- _SpecCubeAlphaMapping_87d1c.cpp ----
extern "C" void _ZN11AbyssEngine20SpecCubeAlphaMappingD0Ev(AbyssEngine::SpecCubeAlphaMapping *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}

// ---- SpecCubeAlphaMapping_87984.cpp ----
// AbyssEngine::SpecCubeAlphaMapping::SpecCubeAlphaMapping()
// Chains the ShaderBaseStruct ctor, installs the vtable, registers the instance in a global,
// and stores the shader's resource-name String at +0xc.
namespace AbyssEngine {

SpecCubeAlphaMapping::SpecCubeAlphaMapping()
{
    ShaderBaseStruct_ctor(this);

    // install vtable (target adds +8 to the table base) and register self in the global slot
    this->field_0x0 = (void *)(SpecCubeAlphaMapping_vtable + 8);
    *g_SCAM_globalSlotDst = *g_SCAM_globalSlotSrc;

    String name;
    ((String *)(&name))->ctor_char(g_SCAM_name, false);
    ((String *)(&this->field_0xc))->assign(&name);
    ((String *)(&name))->dtor();
}

} // namespace AbyssEngine

// ---- SetInActive_87b64.cpp ----
namespace AbyssEngine {

void SpecCubeAlphaMapping::SetInActive()
{
    glDisableVertexAttribArray(this->field_0x20);
    glDisableVertexAttribArray(this->field_0x24);
    return glDisableVertexAttribArray(this->field_0x28);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_87b88.cpp ----
// AbyssEngine::SpecCubeAlphaMapping::UpdateMeshData(Mesh*, Engine*)
// Pushes the per-frame uniforms (env factor, light/material vectors, MVP/normal matrices,
// fog/spec params) into the bound program, then binds the mesh's vertex/uv/normal streams
// (VBO path when the mesh has a buffer object, client-array path otherwise).
namespace AbyssEngine {

void SpecCubeAlphaMapping::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *eng = (char *)engine;
    char *msh = (char *)mesh;

    if (this->field_0x9 != 0) {
        float envFactor = 1.0f;
        int mat = *(int *)(msh + 0x30);
        if (mat != 0 && *(void **)(mat + 0x24) != 0 && *(int *)(mat + 0x28) == 4) {
            envFactor = **(float **)(mat + 0x24);
        }
        glUniform1f(this->field_0x58, envFactor);
        glUniform4fv(this->field_0x54, 1, (const float *)(eng + 0xd0));
        glUniform3fv(this->field_0x44, 1, (const float *)(eng + 0x2cc));
        glUniform3fv(this->field_0x48, 1, (const float *)(eng + 0x2fc));
        glUniform3fv(this->field_0x4c, 1, (const float *)(eng + 0x2e4));
        glUniform1f(this->field_0x50, *(float *)(eng + 0x2c8));
        this->field_0x9 = 0;
    }

    glUniformMatrix4fv(this->field_0x2c, 1, 0, (const float *)(eng + 0x104));
    glUniformMatrix3fv(this->field_0x30, 1, 0, (const float *)(eng + 0x204));
    glUniform4f(this->field_0x38, *(float *)(eng + 0x330), *(float *)(eng + 0x334),
                *(float *)(eng + 0x338), *(float *)(eng + 0x378));
    glUniform3f(this->field_0x34, *(float *)(eng + 0x34c), *(float *)(eng + 0x350),
                *(float *)(eng + 0x354));

    glEnableVertexAttribArray(this->field_0x20);
    glEnableVertexAttribArray(this->field_0x28);
    glEnableVertexAttribArray(this->field_0x24);

    if (*(uint8_t *)(msh + 0x5c) != 0) {
        glBindBuffer(0x8892, *(int *)(msh + 0x60));
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(msh + 0x68));
        glVertexAttribPointer(this->field_0x28, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(msh + 0x6c));
        glVertexAttribPointer(this->field_0x24, 3, 0x1406, 0, 0, 0);
    } else {
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, *(void **)(msh + 0x4));
        uint8_t flags = *(uint8_t *)msh;
        if ((flags & 2) != 0) {
            glVertexAttribPointer(this->field_0x28, 2, 0x1406, 0, 0, *(void **)(msh + 0x8));
            flags = *(uint8_t *)msh;
        }
        if (((unsigned)flags << 0x1d) & 0x80000000u) {
            glVertexAttribPointer(this->field_0x24, 3, 0x1406, 0, 0, *(void **)(msh + 0x10));
        }
    }
}

} // namespace AbyssEngine
