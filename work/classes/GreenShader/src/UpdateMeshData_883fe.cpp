#include "class.h"

namespace AbyssEngine {

void GreenShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (i32(this, 0x34) >= 0)
        glUniformMatrix4fv(i32(this, 0x34), 1, 0, (const float *)((char *)engine + 0x104));

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
        glBindBuffer(0x8892, i32(mesh, 0x60));
        glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, 0);
    } else {
        if (i32(this, 0x20) < 0)
            return;
        glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, (const void *)i32(mesh, 4));
    }
}

} // namespace AbyssEngine
