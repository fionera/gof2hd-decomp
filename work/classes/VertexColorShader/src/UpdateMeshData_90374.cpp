#include "class.h"

typedef void BindBuffer(unsigned int target, unsigned int buffer);
typedef void VertexAttribPointer(unsigned int index, int size, unsigned int type, bool normalized,
                                 int stride, const void *pointer);

extern "C" void glUniformMatrix4fv(int location, int count, bool transpose, const void *value);
extern "C" void glUniformMatrix3fv(int location, int count, bool transpose, const void *value);
extern "C" void glUniform3f(int location, uint32_t x, uint32_t y, uint32_t z);
extern "C" void glUniform4fv(int location, int count, const void *value);
extern "C" void glEnableVertexAttribArray(unsigned int index);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type, bool normalized,
                                      int stride, const void *pointer);

extern "C" __attribute__((visibility("hidden"))) BindBuffer *glBindBuffer_ptr;
extern "C" __attribute__((visibility("hidden"))) VertexAttribPointer *glVertexAttribPointer_ptr;

extern "C" void VertexColorShader_UpdateMeshData(AbyssEngine::VertexColorShader *self,
                                                  AbyssEngine::Mesh *mesh,
                                                  AbyssEngine::Engine *engine)
{
    if (ae_i32(self, 0x38) >= 0) {
        glUniformMatrix4fv(ae_i32(self, 0x38), 1, false, (char *)engine + 0x104);
    }
    if (ae_i32(self, 0x3c) >= 0) {
        glUniformMatrix3fv(ae_i32(self, 0x3c), 1, false, (char *)engine + 0x204);
    }

    if (ae_u8(self, 9) != 0) {
        if (ae_i32(self, 0x40) >= 0) {
            uint32_t *lightDir = (uint32_t *)((char *)engine + 0x330);
            glUniform3f(ae_i32(self, 0x40), lightDir[0], lightDir[1], lightDir[2]);
        }
        if (ae_i32(self, 0x44) >= 0) {
            uint32_t *lightColor = (uint32_t *)((char *)engine + 0x34c);
            glUniform3f(ae_i32(self, 0x44), lightColor[0], lightColor[1], lightColor[2]);
        }
        if (ae_i32(self, 0x4c) >= 0) {
            glUniform4fv(ae_i32(self, 0x4c), 1, (char *)engine + 0xd0);
        }
        if (ae_i32(self, 0x50) >= 0) {
            glUniform4fv(ae_i32(self, 0x50), 1, (char *)engine + 0x2a8);
        }
        if (ae_i32(self, 0x54) >= 0) {
            glUniform4fv(ae_i32(self, 0x54), 1, (char *)engine + 0x298);
        }
        if (ae_i32(self, 0x58) >= 0) {
            glUniform4fv(ae_i32(self, 0x58), 1, (char *)engine + 0x2b8);
        }
        ae_u8(self, 9) = 0;
    }

    if (ae_i32(self, 0x20) >= 0) {
        glEnableVertexAttribArray(ae_u32(self, 0x20));
    }
    if (ae_i32(self, 0x24) >= 0) {
        glEnableVertexAttribArray(ae_u32(self, 0x24));
    }
    if (ae_i32(self, 0x28) >= 0) {
        glEnableVertexAttribArray(ae_u32(self, 0x28));
    }
    if (ae_i32(self, 0x2c) >= 0) {
        glEnableVertexAttribArray(ae_u32(self, 0x2c));
    }
    if (ae_i32(self, 0x30) >= 0) {
        glEnableVertexAttribArray(ae_u32(self, 0x30));
    }
    if (ae_i32(self, 0x34) >= 0) {
        glEnableVertexAttribArray(ae_u32(self, 0x34));
    }

    if (ae_u8(mesh, 0x5c) != 0) {
        BindBuffer *bindBuffer = glBindBuffer_ptr;
        bindBuffer(0x8892, ae_u32(mesh, 0x60));
        VertexAttribPointer *vertexAttribPointer = glVertexAttribPointer_ptr;
        vertexAttribPointer(ae_u32(self, 0x20), 3, 0x1406, false, 0, 0);
        bindBuffer(0x8892, ae_u32(mesh, 0x68));
        vertexAttribPointer(ae_u32(self, 0x24), 2, 0x1406, false, 0, 0);
        bindBuffer(0x8892, ae_u32(mesh, 0x6c));
        vertexAttribPointer(ae_u32(self, 0x28), 3, 0x1406, false, 0, 0);
        bindBuffer(0x8892, ae_u32(mesh, 0x70));
        vertexAttribPointer(ae_u32(self, 0x2c), 3, 0x1406, false, 0, 0);
        bindBuffer(0x8892, ae_u32(mesh, 0x74));
        vertexAttribPointer(ae_u32(self, 0x30), 3, 0x1406, false, 0, 0);
        bindBuffer(0x8892, ae_u32(mesh, 0x78));
        return vertexAttribPointer(ae_u32(self, 0x34), 4, 0x1406, false, 0, 0);
    }

    if (ae_i32(self, 0x20) >= 0) {
        glVertexAttribPointer(ae_u32(self, 0x20), 3, 0x1406, false, 0, ae_ptr(mesh, 4));
    }
    if (ae_i32(self, 0x24) >= 0) {
        glVertexAttribPointer(ae_u32(self, 0x24), 2, 0x1406, false, 0, ae_ptr(mesh, 8));
    }
    if (ae_i32(self, 0x28) >= 0) {
        glVertexAttribPointer(ae_u32(self, 0x28), 3, 0x1406, false, 0, ae_ptr(mesh, 0x10));
    }
    if (ae_i32(self, 0x2c) >= 0) {
        glVertexAttribPointer(ae_u32(self, 0x2c), 3, 0x1406, false, 0, ae_ptr(mesh, 0x14));
    }
    if (ae_i32(self, 0x30) >= 0) {
        glVertexAttribPointer(ae_u32(self, 0x30), 3, 0x1406, false, 0, ae_ptr(mesh, 0x18));
    }
    if (ae_i32(self, 0x34) >= 0) {
        glVertexAttribPointer(ae_u32(self, 0x34), 4, 0x1406, false, 0, ae_ptr(mesh, 0x0c));
    }
}
