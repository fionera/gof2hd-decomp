#include "class.h"

namespace AbyssEngine {

void SandboxShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x104);

    if (field_u8(this, 0x9) != 0) {
        glUniform4fv(field_i32(this, 0x40), 1, (float *)((char *)engine + 0xd0));
        glUniform3f(field_i32(this, 0x38), field_f32(engine, 0x330), field_f32(engine, 0x334),
                    field_f32(engine, 0x338));
        glUniform3f(field_i32(this, 0x3c), field_f32(engine, 0x34c), field_f32(engine, 0x350),
                    field_f32(engine, 0x354));
        field_u8(this, 0x9) = 0;
    }

    void (*enableVertexAttribArray)(uint32_t) = glEnableVertexAttribArray;
    enableVertexAttribArray(field_i32(this, 0x20));
    enableVertexAttribArray(field_i32(this, 0x24));
    enableVertexAttribArray(field_i32(this, 0x28));
    enableVertexAttribArray(field_i32(this, 0x2c));
    enableVertexAttribArray(field_i32(this, 0x30));

    if (field_u8(mesh, 0x5c) != 0) {
        void (*bindBuffer)(uint32_t, uint32_t) = glBindBuffer;
        void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, field_i32(mesh, 0x60));
        vertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x68));
        vertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x6c));
        vertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x70));
        vertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x74));
        vertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, 0);
        return;
    }

    void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
        glVertexAttribPointer;
    vertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    vertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
    vertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
    vertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
    vertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
}

} // namespace AbyssEngine
