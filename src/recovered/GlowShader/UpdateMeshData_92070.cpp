#include "class.h"

namespace AbyssEngine {

void GlowShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x28) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x28), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x2c) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x2c), 1, 0, (char *)engine + 0x204);

    if (field_i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x20));
    if (field_i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x24));

    if (field_u8(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        return glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
    }
    if (field_i32(this, 0x20) >= 0)
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    int index = field_i32(this, 0x24);
    if (index < 0)
        return;
    return glVertexAttribPointer(index, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
}

} // namespace AbyssEngine
