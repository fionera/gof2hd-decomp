#include "class.h"

namespace AbyssEngine {

// AbyssEngine::GenericShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void GenericShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    if (i32(this, 0x34) >= 0)
        glUniformMatrix4fv(i32(this, 0x34), 1, 0, (const float *)(e + 0x104));
    if (i32(this, 0x38) >= 0)
        glUniformMatrix3fv(i32(this, 0x38), 1, 0, (const float *)(e + 0x204));

    if (u8(this, 0x9) != 0) {
        if (i32(this, 0x3c) >= 0)
            glUniform3f(i32(this, 0x3c), f32(e, 0x330), f32(e, 0x334), f32(e, 0x338));
        if (i32(this, 0x40) >= 0)
            glUniform3f(i32(this, 0x40), f32(e, 0x34c), f32(e, 0x350), f32(e, 0x354));
        if (i32(this, 0x50) >= 0)
            glUniform4fv(i32(this, 0x50), 1, (const float *)(e + 0xd0));
        if (i32(this, 0x54) >= 0)
            glUniform3fv(i32(this, 0x54), 1, (const float *)(e + 0x2cc));
        if (i32(this, 0x58) >= 0)
            glUniform3fv(i32(this, 0x58), 1, (const float *)(e + 0x2fc));
        if (i32(this, 0x5c) >= 0)
            glUniform3fv(i32(this, 0x5c), 1, (const float *)(e + 0x2e4));
        if (i32(this, 0x60) >= 0)
            glUniform1f(i32(this, 0x60), f32(e, 0x2c8));
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

    if (u8(mesh, 0x5c) != 0) {
        void (*bindBuffer)(unsigned int, unsigned int) = glBindBuffer;
        void (*vertexAttribPointer)(int, int, unsigned int, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, i32(m, 0x60));
        vertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(m, 0x68));
        vertexAttribPointer(i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(m, 0x6c));
        vertexAttribPointer(i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(m, 0x70));
        vertexAttribPointer(i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(m, 0x74));
        vertexAttribPointer(i32(this, 0x30), 3, 0x1406, 0, 0, 0);
        return;
    }

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
}

} // namespace AbyssEngine
