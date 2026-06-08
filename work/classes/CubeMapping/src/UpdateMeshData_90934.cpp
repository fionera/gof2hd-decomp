#include "class.h"

namespace AbyssEngine {

void CubeMapping::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (u8(this, 0x9) != 0) {
        void (*uniform4fv)(int, int, const float *) = glUniform4fv;
        uniform4fv(i32(this, 0x58), 1, (float *)((char *)engine + 0xd0));
        uniform4fv(i32(this, 0x48), 1, (float *)((char *)engine + 0x2cc));
        uniform4fv(i32(this, 0x4c), 1, (float *)((char *)engine + 0x2fc));
        uniform4fv(i32(this, 0x50), 1, (float *)((char *)engine + 0x2e4));
        glUniform1f(i32(this, 0x54), f32(engine, 0x2c8));
        u8(this, 0x9) = 0;
    }

    glUniform1f(i32(this, 0x44), f32(engine, 0xcc));
    glUniformMatrix4fv(i32(this, 0x2c), 1, 0, (float *)((char *)engine + 0x104));
    glUniformMatrix3fv(i32(this, 0x30), 1, 0, (float *)((char *)engine + 0x204));
    glUniform4f(i32(this, 0x38), f32(engine, 0x330), f32(engine, 0x334), f32(engine, 0x338),
                f32(engine, 0x378));
    glUniform3f(i32(this, 0x34), f32(engine, 0x34c), f32(engine, 0x350), f32(engine, 0x354));

    void (*enableVertexAttribArray)(int) = glEnableVertexAttribArray;
    enableVertexAttribArray(i32(this, 0x20));
    enableVertexAttribArray(i32(this, 0x28));
    enableVertexAttribArray(i32(this, 0x24));

    if (u8(mesh, 0x5c) == 0) {
        glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0,
                              *(void **)((char *)mesh + 0x4));
        if ((u8(mesh, 0x0) & 2) != 0)
            glVertexAttribPointer(i32(this, 0x28), 2, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x8));
        if ((u8(mesh, 0x0) & 4) != 0)
            glVertexAttribPointer(i32(this, 0x24), 3, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x10));
    } else {
        void (*bindBuffer)(unsigned int, unsigned int) = glBindBuffer;
        void (*vertexAttribPointer)(int, int, unsigned int, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, i32(mesh, 0x60));
        vertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(mesh, 0x68));
        vertexAttribPointer(i32(this, 0x28), 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(mesh, 0x6c));
        vertexAttribPointer(i32(this, 0x24), 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
