#include "gof2/SpecCubeAlphaMapping.h"
#include "gof2/String.h"


// ---- Init_87a10.cpp ----
namespace AbyssEngine {

void SpecCubeAlphaMapping::Init(Engine *)
{
    int program = ShaderBaseStruct_LoadBindShader(this, "SpecCubeAlphaMapping.vsh", "SpecCubeAlphaMapping.fsh");
    this->program = program;
    if (program == 0) {
        program = ShaderBaseStruct_ES2LoadProgram(this, "SpecCubeAlphaMapping.vsh", "SpecCubeAlphaMapping2.fsh");
        this->program = program;
    }

    this->attrA0 = glGetAttribLocation(program, "a0");
    this->attrA1 = glGetAttribLocation(this->program, "a1");
    this->attrA2 = glGetAttribLocation(this->program, "a2");

    this->uniU0 = glGetUniformLocation(this->program, "u0");
    this->uniU1 = glGetUniformLocation(this->program, "u1");
    this->uniU2 = glGetUniformLocation(this->program, "u2");
    this->uniU3 = glGetUniformLocation(this->program, "u3");
    this->uniU4 = glGetUniformLocation(this->program, "u4");
    this->uniU5 = glGetUniformLocation(this->program, "u5");
    this->uniU6 = glGetUniformLocation(this->program, "u6");
    this->uniU7 = glGetUniformLocation(this->program, "u7");
    this->uniU8 = glGetUniformLocation(this->program, "u8");
    this->uniU9 = glGetUniformLocation(this->program, "u9");
    this->uniU10 = glGetUniformLocation(this->program, "u10");
    this->uniU11 = glGetUniformLocation(this->program, "u11");

    glUseProgram(this->program);
    glUniform1i(this->uniU4, 0);
    return glUniform1i(this->uniU5, 1);
}

} // namespace AbyssEngine

// ---- _SpecCubeAlphaMapping_87d1c.cpp ----
void _ZN11AbyssEngine20SpecCubeAlphaMappingD0Ev(AbyssEngine::SpecCubeAlphaMapping *self)
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
    ((String *)(&this->name))->assign(&name);
    ((String *)(&name))->dtor();
}

} // namespace AbyssEngine

// ---- SetInActive_87b64.cpp ----
namespace AbyssEngine {

void SpecCubeAlphaMapping::SetInActive()
{
    glDisableVertexAttribArray(this->attrA0);
    glDisableVertexAttribArray(this->attrA1);
    return glDisableVertexAttribArray(this->attrA2);
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

    if (this->uniformsDirty != 0) {
        float envFactor = 1.0f;
        int mat = *(int *)(msh + 0x30);
        if (mat != 0 && *(void **)(mat + 0x24) != 0 && *(int *)(mat + 0x28) == 4) {
            envFactor = **(float **)(mat + 0x24);
        }
        glUniform1f(this->uniU11, envFactor);
        glUniform4fv(this->uniU10, 1, (const float *)(eng + 0xd0));
        glUniform3fv(this->uniU6, 1, (const float *)(eng + 0x2cc));
        glUniform3fv(this->uniU7, 1, (const float *)(eng + 0x2fc));
        glUniform3fv(this->uniU8, 1, (const float *)(eng + 0x2e4));
        glUniform1f(this->uniU9, *(float *)(eng + 0x2c8));
        this->uniformsDirty = 0;
    }

    glUniformMatrix4fv(this->uniU0, 1, 0, (const float *)(eng + 0x104));
    glUniformMatrix3fv(this->uniU1, 1, 0, (const float *)(eng + 0x204));
    glUniform4f(this->uniU3, *(float *)(eng + 0x330), *(float *)(eng + 0x334),
                *(float *)(eng + 0x338), *(float *)(eng + 0x378));
    glUniform3f(this->uniU2, *(float *)(eng + 0x34c), *(float *)(eng + 0x350),
                *(float *)(eng + 0x354));

    glEnableVertexAttribArray(this->attrA0);
    glEnableVertexAttribArray(this->attrA2);
    glEnableVertexAttribArray(this->attrA1);

    if (*(uint8_t *)(msh + 0x5c) != 0) {
        glBindBuffer(0x8892, *(int *)(msh + 0x60));
        glVertexAttribPointer(this->attrA0, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(msh + 0x68));
        glVertexAttribPointer(this->attrA2, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(msh + 0x6c));
        glVertexAttribPointer(this->attrA1, 3, 0x1406, 0, 0, 0);
    } else {
        glVertexAttribPointer(this->attrA0, 3, 0x1406, 0, 0, *(void **)(msh + 0x4));
        uint8_t flags = *(uint8_t *)msh;
        if ((flags & 2) != 0) {
            glVertexAttribPointer(this->attrA2, 2, 0x1406, 0, 0, *(void **)(msh + 0x8));
            flags = *(uint8_t *)msh;
        }
        if (((unsigned)flags << 0x1d) & 0x80000000u) {
            glVertexAttribPointer(this->attrA1, 3, 0x1406, 0, 0, *(void **)(msh + 0x10));
        }
    }
}

} // namespace AbyssEngine
