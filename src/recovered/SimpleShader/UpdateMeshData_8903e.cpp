#include "class.h"

namespace AbyssEngine {

void SimpleShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_i32(this, 0x24), 1, 0, bytes(engine) + 0x104);
    if (field_u8(this, 0x9) != 0) {
        glUniform4fv(field_i32(this, 0x28), 1, (const float *)(bytes(engine) + 0xd0));
        field_u8(this, 0x9) = 0;
    }
    glEnableVertexAttribArray(field_u32(this, 0x20));
    if (field_u8(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, field_u32(mesh, 0x60));
        return glVertexAttribPointer(field_u32(this, 0x20), 3, 0x1406, 0, 0, 0);
    }
    return glVertexAttribPointer(field_u32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
}

} // namespace AbyssEngine
