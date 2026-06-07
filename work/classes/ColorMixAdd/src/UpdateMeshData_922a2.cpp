#include "class.h"

namespace AbyssEngine {

void ColorMixAdd::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_i32(this, 0x28), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x34) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x1c4);
    if (field_i32(this, 0x3c) >= 0)
        glUniform1i(field_i32(this, 0x3c), field_u8(mesh, 0x85));

    if (field_u8(this, 0x9) != 0) {
        glUniform4fv(field_i32(this, 0x2c), 1, (float *)((char *)engine + 0xd0));
        if (field_i32(this, 0x38) >= 0)
            glUniform1f(field_i32(this, 0x38), 1.0f - field_f32(mesh, 0x1c));
        field_u8(this, 0x9) = 0;
    }

    if ((field_u8(mesh, 0x0) & 2) != 0) {
        glEnableVertexAttribArray(field_i32(this, 0x20));
        glEnableVertexAttribArray(field_i32(this, 0x24));
        if (field_u8(mesh, 0x5c) == 0) {
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        } else {
            glBindBuffer(0x8892, field_i32(mesh, 0x60));
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, field_i32(mesh, 0x68));
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        }
    }
}

} // namespace AbyssEngine
