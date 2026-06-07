#include "class.h"

namespace AbyssEngine {

void TexOnlyShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field<int>(this, 0x28), 1, 0, (char *)engine + 0x104);
    if (field<uint8_t>(this, 0x9) != 0) {
        field<uint8_t>(this, 0x9) = 0;
    }
    glEnableVertexAttribArray(field<uint32_t>(this, 0x20));
    glEnableVertexAttribArray(field<uint32_t>(this, 0x24));

    if (field<uint8_t>(mesh, 0x5c) == 0) {
        glVertexAttribPointer(field<uint32_t>(this, 0x20), 3, 0x1406, 0, 0, field<void *>(mesh, 0x4));
        return glVertexAttribPointer(field<uint32_t>(this, 0x24), 2, 0x1406, 0, 0, field<void *>(mesh, 0x8));
    } else {
        glBindBuffer(0x8892, field<uint32_t>(mesh, 0x60));
        glVertexAttribPointer(field<uint32_t>(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field<uint32_t>(mesh, 0x68));
        return glVertexAttribPointer(field<uint32_t>(this, 0x24), 2, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
