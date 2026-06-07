#include "class.h"

extern "C" void glUniformMatrix4fv(int location, int count, unsigned char transpose,
                                   const void *value);
extern "C" void glEnableVertexAttribArray(unsigned int index);
extern "C" void glBindBuffer(unsigned int target, unsigned int buffer);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type,
                                      unsigned char normalized, int stride, const void *pointer);

namespace AbyssEngine {

void BloomShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(*(int *)((char *)this + 0x90), 1, 0, (char *)engine + 0x104);
    if (*(uint8_t *)((char *)this + 0x9) != 0) {
        *(uint8_t *)((char *)this + 0x9) = 0;
    }

    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x8c));
    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x94));

    unsigned int zero = 0;
    if (*(uint8_t *)((char *)mesh + 0x5c) != 0) {
        glBindBuffer(0x8892, *(unsigned int *)((char *)mesh + 0x60));
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x8c), 3, 0x1406, 0, 0,
                              (void *)zero);
        glBindBuffer(0x8892, *(unsigned int *)((char *)mesh + 0x68));
        return glVertexAttribPointer(*(unsigned int *)((char *)this + 0x94), 2, 0x1406, 0, 0,
                                     (void *)zero);
    } else {
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x8c), 3, 0x1406, 0, 0,
                              *(void **)((char *)mesh + 0x4));
        return glVertexAttribPointer(*(unsigned int *)((char *)this + 0x94), 2, 0x1406, 0, 0,
                                     *(void **)((char *)mesh + 0x8));
    }
}

} // namespace AbyssEngine
