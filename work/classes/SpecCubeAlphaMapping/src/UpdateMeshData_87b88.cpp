#include "class.h"

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

    if (u8(this, 0x9) != 0) {
        float envFactor = 1.0f;
        int mat = *(int *)(msh + 0x30);
        if (mat != 0 && *(void **)(mat + 0x24) != 0 && *(int *)(mat + 0x28) == 4) {
            envFactor = **(float **)(mat + 0x24);
        }
        glUniform1f(i32(this, 0x58), envFactor);
        glUniform4fv(i32(this, 0x54), 1, (const float *)(eng + 0xd0));
        glUniform3fv(i32(this, 0x44), 1, (const float *)(eng + 0x2cc));
        glUniform3fv(i32(this, 0x48), 1, (const float *)(eng + 0x2fc));
        glUniform3fv(i32(this, 0x4c), 1, (const float *)(eng + 0x2e4));
        glUniform1f(i32(this, 0x50), *(float *)(eng + 0x2c8));
        u8(this, 0x9) = 0;
    }

    glUniformMatrix4fv(i32(this, 0x2c), 1, 0, (const float *)(eng + 0x104));
    glUniformMatrix3fv(i32(this, 0x30), 1, 0, (const float *)(eng + 0x204));
    glUniform4f(i32(this, 0x38), *(float *)(eng + 0x330), *(float *)(eng + 0x334),
                *(float *)(eng + 0x338), *(float *)(eng + 0x378));
    glUniform3f(i32(this, 0x34), *(float *)(eng + 0x34c), *(float *)(eng + 0x350),
                *(float *)(eng + 0x354));

    glEnableVertexAttribArray(i32(this, 0x20));
    glEnableVertexAttribArray(i32(this, 0x28));
    glEnableVertexAttribArray(i32(this, 0x24));

    if (*(uint8_t *)(msh + 0x5c) != 0) {
        glBindBuffer(0x8892, *(int *)(msh + 0x60));
        glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(msh + 0x68));
        glVertexAttribPointer(i32(this, 0x28), 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(msh + 0x6c));
        glVertexAttribPointer(i32(this, 0x24), 3, 0x1406, 0, 0, 0);
    } else {
        glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, *(void **)(msh + 0x4));
        uint8_t flags = *(uint8_t *)msh;
        if ((flags & 2) != 0) {
            glVertexAttribPointer(i32(this, 0x28), 2, 0x1406, 0, 0, *(void **)(msh + 0x8));
            flags = *(uint8_t *)msh;
        }
        if (((unsigned)flags << 0x1d) & 0x80000000u) {
            glVertexAttribPointer(i32(this, 0x24), 3, 0x1406, 0, 0, *(void **)(msh + 0x10));
        }
    }
}

} // namespace AbyssEngine
