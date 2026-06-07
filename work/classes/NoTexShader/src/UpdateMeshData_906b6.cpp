#include "class.h"

namespace AbyssEngine {

void NoTexShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_i32(this, 0x24), 1, 0, (char *)engine + 0x104);
    if (field_u8(this, 0x9) != 0) {
        glUniform4fv(field_i32(this, 0x28), 1, (float *)((char *)engine + 0xd0));
        field_u8(this, 0x9) = 0;
    }
    glEnableVertexAttribArray(field_i32(this, 0x20));

    int index, size;
    const void *ptr;
    if (mesh == 0) {
        ptr = field_ptr(engine, 0x348);
        index = field_i32(this, 0x20);
        size = 2;
    } else {
        if (field_u8(mesh, 0x5c) == 0) {
            index = field_i32(this, 0x20);
            ptr = field_ptr(mesh, 0x4);
        } else {
            glBindBuffer(0x8892, field_i32(mesh, 0x60));
            ptr = 0;
            index = field_i32(this, 0x20);
        }
        size = 3;
    }
    return glVertexAttribPointer(index, size, 0x1406, 0, 0, ptr);
}

} // namespace AbyssEngine
