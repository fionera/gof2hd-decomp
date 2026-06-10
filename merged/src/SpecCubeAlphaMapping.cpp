#include "SpecCubeAlphaMapping.h"




// ---- Init_87a10.cpp ----
namespace AbyssEngine {

void SpecCubeAlphaMapping::Init(Engine *)
{
    int program = ShaderBaseStruct_LoadBindShader(this, "SpecCubeAlphaMapping.vsh", "SpecCubeAlphaMapping.fsh");
    i32(this, 0x04) = program;
    if (program == 0) {
        program = ShaderBaseStruct_ES2LoadProgram(this, "SpecCubeAlphaMapping.vsh", "SpecCubeAlphaMapping2.fsh");
        i32(this, 0x04) = program;
    }

    i32(this, 0x20) = glGetAttribLocation(program, "a0");
    i32(this, 0x24) = glGetAttribLocation(i32(this, 0x04), "a1");
    i32(this, 0x28) = glGetAttribLocation(i32(this, 0x04), "a2");

    i32(this, 0x2c) = glGetUniformLocation(i32(this, 0x04), "u0");
    i32(this, 0x30) = glGetUniformLocation(i32(this, 0x04), "u1");
    i32(this, 0x34) = glGetUniformLocation(i32(this, 0x04), "u2");
    i32(this, 0x38) = glGetUniformLocation(i32(this, 0x04), "u3");
    i32(this, 0x40) = glGetUniformLocation(i32(this, 0x04), "u4");
    i32(this, 0x3c) = glGetUniformLocation(i32(this, 0x04), "u5");
    i32(this, 0x44) = glGetUniformLocation(i32(this, 0x04), "u6");
    i32(this, 0x48) = glGetUniformLocation(i32(this, 0x04), "u7");
    i32(this, 0x4c) = glGetUniformLocation(i32(this, 0x04), "u8");
    i32(this, 0x50) = glGetUniformLocation(i32(this, 0x04), "u9");
    i32(this, 0x54) = glGetUniformLocation(i32(this, 0x04), "u10");
    i32(this, 0x58) = glGetUniformLocation(i32(this, 0x04), "u11");

    glUseProgram(i32(this, 0x04));
    glUniform1i(i32(this, 0x40), 0);
    return glUniform1i(i32(this, 0x3c), 1);
}

} // namespace AbyssEngine

// ---- _SpecCubeAlphaMapping_87d1c.cpp ----
extern "C" void _ZN11AbyssEngine20SpecCubeAlphaMappingD0Ev(SpecCubeAlphaMapping *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}

// ---- SpecCubeAlphaMapping_87984.cpp ----
// AbyssEngine::SpecCubeAlphaMapping::SpecCubeAlphaMapping()
// Chains the ShaderBaseStruct ctor, installs the vtable, registers the instance in a global,
// and stores the shader's resource-name String at +0xc.
extern "C" {
extern void **g_SCAM_globalSlotSrc;   // *(undefined4**)(DAT_00097a00...)
extern void **g_SCAM_globalSlotDst;   // *(undefined4**)(DAT_00097a08...)
extern const char g_SCAM_name[];      // resource-name string literal
}

namespace AbyssEngine {

SpecCubeAlphaMapping::SpecCubeAlphaMapping()
{
    ShaderBaseStruct_ctor(this);

    // install vtable (target adds +8 to the table base) and register self in the global slot
    this->f_0 = (int)(SpecCubeAlphaMapping_vtable + 8);
    *g_SCAM_globalSlotDst = *g_SCAM_globalSlotSrc;

    String name(g_SCAM_name, false);
    *(String *)((char *)this + 0xc) = name;
}

} // namespace AbyssEngine

// ---- SetInActive_87b64.cpp ----
namespace AbyssEngine {

void SpecCubeAlphaMapping::SetInActive()
{
    glDisableVertexAttribArray(this->f_20);
    glDisableVertexAttribArray(this->f_24);
    return glDisableVertexAttribArray(this->f_28);
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
    char *self = (char *)this;
    char *eng = (char *)engine;
    char *msh = (char *)mesh;

    if (this->f_9 != 0) {
        float envFactor = 1.0f;
        int mat = *(int *)(msh + 0x30);
        if (mat != 0 && *(void **)(mat + 0x24) != 0 && *(int *)(mat + 0x28) == 4) {
            envFactor = **(float **)(mat + 0x24);
        }
        glUniform1f(this->f_58, envFactor);
        glUniform4fv(this->f_54, 1, (const float *)(eng + 0xd0));
        glUniform3fv(this->f_44, 1, (const float *)(eng + 0x2cc));
        glUniform3fv(this->f_48, 1, (const float *)(eng + 0x2fc));
        glUniform3fv(this->f_4c, 1, (const float *)(eng + 0x2e4));
        glUniform1f(this->f_50, *(float *)(eng + 0x2c8));
        this->f_9 = 0;
    }

    glUniformMatrix4fv(this->f_2c, 1, 0, (const float *)(eng + 0x104));
    glUniformMatrix3fv(this->f_30, 1, 0, (const float *)(eng + 0x204));
    glUniform4f(this->f_38, *(float *)(eng + 0x330), *(float *)(eng + 0x334),
                *(float *)(eng + 0x338), *(float *)(eng + 0x378));
    glUniform3f(this->f_34, *(float *)(eng + 0x34c), *(float *)(eng + 0x350),
                *(float *)(eng + 0x354));

    glEnableVertexAttribArray(this->f_20);
    glEnableVertexAttribArray(this->f_28);
    glEnableVertexAttribArray(this->f_24);

    if (*(uint8_t *)(msh + 0x5c) != 0) {
        glBindBuffer(0x8892, *(int *)(msh + 0x60));
        glVertexAttribPointer(this->f_20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(msh + 0x68));
        glVertexAttribPointer(this->f_28, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(msh + 0x6c));
        glVertexAttribPointer(this->f_24, 3, 0x1406, 0, 0, 0);
    } else {
        glVertexAttribPointer(this->f_20, 3, 0x1406, 0, 0, *(void **)(msh + 0x4));
        uint8_t flags = *(uint8_t *)msh;
        if ((flags & 2) != 0) {
            glVertexAttribPointer(this->f_28, 2, 0x1406, 0, 0, *(void **)(msh + 0x8));
            flags = *(uint8_t *)msh;
        }
        if (((unsigned)flags << 0x1d) & 0x80000000u) {
            glVertexAttribPointer(this->f_24, 3, 0x1406, 0, 0, *(void **)(msh + 0x10));
        }
    }
}

} // namespace AbyssEngine
