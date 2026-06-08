#include "class.h"

// Additional GL entry points used by this shader's per-mesh upload.
extern "C" void glUniform3fv(int location, int count, const float *value);
extern "C" void glUniformMatrix3fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glUniform4f(int location, float x, float y, float z, float w);

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
