#include "class.h"

extern "C" void _ZN11AbyssEngine6Engine14SetTextureSlotEjj(
    AbyssEngine::Engine *self, uint32_t a, uint32_t b);

namespace AbyssEngine {

void MaskShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x2c) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x2c), 1, 0, (char *)engine + 0x104);

    if (field_u8(this, 0x9) != 0) {
        if (field_i32(this, 0x38) >= 0)
            glUniform4fv(field_i32(this, 0x38), 1, (float *)((char *)engine + 0xd0));
        field_u8(this, 0x9) = 0;
    }

    if (field_i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x20));
    if (field_i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x24));
    if (field_i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x28));

    char *texBase = *(char **)(*(char **)((char *)engine + 0x30));
    int *tex = *(int **)(texBase + 0x20);
    if (tex != 0)
        _ZN11AbyssEngine6Engine14SetTextureSlotEjj(engine, tex[1], 1);

    if (field_u8(mesh, 0x5c) == 0) {
        if (field_i32(this, 0x20) >= 0)
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (field_i32(this, 0x24) >= 0)
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (tex == 0)
            return;
        if (field_i32(this, 0x28) < 0)
            return;
        glVertexAttribPointer(field_i32(this, 0x28), 2, 0x1406, 0, 0, *(void **)(*tex + 8));
    } else {
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        if (tex == 0)
            return;
        glBindBuffer(0x8892, *(int *)(*tex + 0x68));
        glVertexAttribPointer(field_i32(this, 0x28), 2, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
