#include "class.h"

namespace AbyssEngine {

// AbyssEngine::BumpShaderV2::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void BumpShaderV2::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *m = (char *)mesh;
    char *e = (char *)engine;

    if (i32(this, 0x38) >= 0)
        glUniformMatrix4fv(i32(this, 0x38), 1, 0, (const float *)(e + 0x104));
    if (i32(this, 0x3c) >= 0)
        glUniformMatrix3fv(i32(this, 0x3c), 1, 0, (const float *)(e + 0x204));

    // Per-material lighting block, uploaded once (gated by the +0x9 dirty flag).
    if (u8(this, 0x9)) {
        if (i32(this, 0x40) >= 0)
            glUniform3f(i32(this, 0x40), f32(e, 0x330), f32(e, 0x334), f32(e, 0x338));
        if (i32(this, 0x44) >= 0)
            glUniform3f(i32(this, 0x44), f32(e, 0x34c), f32(e, 0x350), f32(e, 0x354));
        if (i32(this, 0x50) >= 0)
            glUniform4fv(i32(this, 0x50), 1, (const float *)(e + 0xd0));
        if (i32(this, 0x54) >= 0)
            glUniform4fv(i32(this, 0x54), 1, (const float *)(e + 0x2a8));
        if (i32(this, 0x58) >= 0)
            glUniform4fv(i32(this, 0x58), 1, (const float *)(e + 0x298));
        if (i32(this, 0x5c) >= 0)
            glUniform4fv(i32(this, 0x5c), 1, (const float *)(e + 0x2b8));
        u8(this, 0x9) = 0;
    }

    if (i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(i32(this, 0x20));
    if (i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(i32(this, 0x24));
    if (i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(i32(this, 0x28));
    if (i32(this, 0x2c) >= 0)
        glEnableVertexAttribArray(i32(this, 0x2c));
    if (i32(this, 0x30) >= 0)
        glEnableVertexAttribArray(i32(this, 0x30));

    if (u8(m, 0x5c) == 0) {
        // Client-side vertex arrays.
        if (i32(this, 0x20) >= 0)
            glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (i32(this, 0x24) >= 0)
            glVertexAttribPointer(i32(this, 0x24), 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        if (i32(this, 0x28) >= 0)
            glVertexAttribPointer(i32(this, 0x28), 3, 0x1406, 0, 0, *(void **)(m + 0x10));
        if (i32(this, 0x2c) >= 0)
            glVertexAttribPointer(i32(this, 0x2c), 3, 0x1406, 0, 0, *(void **)(m + 0x14));
        if (i32(this, 0x30) >= 0)
            glVertexAttribPointer(i32(this, 0x30), 3, 0x1406, 0, 0, *(void **)(m + 0x18));
    } else {
        // VBO path: bind each buffer and use zero offset.
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x60));
        glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x68));
        glVertexAttribPointer(i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x6c));
        glVertexAttribPointer(i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x70));
        glVertexAttribPointer(i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x74));
        glVertexAttribPointer(i32(this, 0x30), 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
