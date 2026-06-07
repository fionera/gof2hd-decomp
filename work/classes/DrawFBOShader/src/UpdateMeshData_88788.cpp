#include "class.h"

namespace AbyssEngine {

void DrawFBOShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *self = (char *)this;
    char *meshBytes = (char *)mesh;
    glUniformMatrix4fv(*(int32_t *)(self + 0x4c), 1, 0, (char *)engine + 0x104);
    if (*(uint8_t *)(self + 0x9) != 0) {
        *(uint8_t *)(self + 0x9) = 0;
    }

    glEnableVertexAttribArray(*(uint32_t *)(self + 0x48));
    glEnableVertexAttribArray(*(uint32_t *)(self + 0x50));

    if (*(uint8_t *)(meshBytes + 0x5c) == 0) {
        glVertexAttribPointer(*(uint32_t *)(self + 0x48), 3, 0x1406, 0, 0,
                              *(void **)(meshBytes + 0x4));
        return glVertexAttribPointer(*(uint32_t *)(self + 0x50), 2, 0x1406, 0, 0,
                                     *(void **)(meshBytes + 0x8));
    } else {
        glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x60));
        glVertexAttribPointer(*(uint32_t *)(self + 0x48), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x68));
        return glVertexAttribPointer(*(uint32_t *)(self + 0x50), 2, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
