#include "class.h"

extern "C" void GlowPPShader_UpdateMeshData(GlowPPShader *self, Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_u32(self, 0x80), 1, 0, (char *)engine + 0x104);
    if (field_u8(self, 0x9) != 0) {
        field_u8(self, 0x9) = 0;
    }

    glEnableVertexAttribArray(field_u32(self, 0x90));
    glEnableVertexAttribArray(field_u32(self, 0x98));
    if (field_u8(mesh, 0x5c) == 0) {
        glVertexAttribPointer(field_u32(self, 0x90), 3, 0x1406, 0, 0, (void *)field_u32(mesh, 4));
        glVertexAttribPointer(field_u32(self, 0x98), 2, 0x1406, 0, 0, (void *)field_u32(mesh, 8));
    } else {
        glBindBuffer(0x8892, field_u32(mesh, 0x60));
        glVertexAttribPointer(field_u32(self, 0x90), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_u32(mesh, 0x68));
        glVertexAttribPointer(field_u32(self, 0x98), 2, 0x1406, 0, 0, 0);
    }
}
