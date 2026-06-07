#include "class.h"

namespace AbyssEngine {

void BumpMapping::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_u8(this, 0x9) != 0)
        field_u8(this, 0x9) = 0;

    glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x104);
    glUniform3f(field_i32(this, 0x38), field_f32(engine, 0x330), field_f32(engine, 0x334),
                field_f32(engine, 0x338));

    void (*enableVertexAttribArray)(uint32_t) = glEnableVertexAttribArray;
    enableVertexAttribArray(field_i32(this, 0x20));
    enableVertexAttribArray(field_i32(this, 0x24));
    enableVertexAttribArray(field_i32(this, 0x28));
    enableVertexAttribArray(field_i32(this, 0x2c));
    enableVertexAttribArray(field_i32(this, 0x30));

    glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    if ((field_u8(mesh, 0x0) & 2) != 0) {
        glVertexAttribPointer(field_i32(this, 0x30), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
    }
    if ((field_u8(mesh, 0x0) & 4) != 0) {
        void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
            glVertexAttribPointer;
        vertexAttribPointer(field_i32(this, 0x24), 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        vertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        vertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    }
}

} // namespace AbyssEngine
