#include "class.h"

namespace AbyssEngine {

void NoTexVtxColorShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_i32(this, 0x28), 1, 0, (char *)engine + 0x104);
    if (field_u8(this, 0x9) != 0) {
        glUniform4fv(field_i32(this, 0x2c), 1, (float *)((char *)engine + 0xd0));
        field_u8(this, 0x9) = 0;
    }
    glEnableVertexAttribArray(field_i32(this, 0x20));
    glEnableVertexAttribArray(field_i32(this, 0x24));

    int index = field_i32(this, 0x20);
    int size;
    const void *ptr;
    int stride = 0;
    if (mesh == 0) {
        ptr = field_ptr(engine, 0x348);
        size = 2;
    } else {
        glVertexAttribPointer(index, 3, 0x1406, 0, stride, field_ptr(mesh, 0x4));
        ptr = field_ptr(mesh, 0xc);
        index = field_i32(this, 0x24);
        size = 4;
    }
    return glVertexAttribPointer(index, size, 0x1406, 0, stride, ptr);
}

} // namespace AbyssEngine
