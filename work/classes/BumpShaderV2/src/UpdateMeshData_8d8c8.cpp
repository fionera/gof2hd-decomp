#include "class.h"

namespace AbyssEngine {

// AbyssEngine::BumpShaderV2::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void BumpShaderV2::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *m = (char *)mesh;
    char *e = (char *)engine;

    if (this->f_38 >= 0)
        glUniformMatrix4fv(this->f_38, 1, 0, (const float *)(e + 0x104));
    if (this->f_3c >= 0)
        glUniformMatrix3fv(this->f_3c, 1, 0, (const float *)(e + 0x204));

    // Per-material lighting block, uploaded once (gated by the +0x9 dirty flag).
    if (this->f_9) {
        if (this->f_40 >= 0)
            glUniform3f(this->f_40, e->f_330, e->f_334, e->f_338);
        if (this->f_44 >= 0)
            glUniform3f(this->f_44, e->f_34c, e->f_350, e->f_354);
        if (this->f_50 >= 0)
            glUniform4fv(this->f_50, 1, (const float *)(e + 0xd0));
        if (this->f_54 >= 0)
            glUniform4fv(this->f_54, 1, (const float *)(e + 0x2a8));
        if (this->f_58 >= 0)
            glUniform4fv(this->f_58, 1, (const float *)(e + 0x298));
        if (this->f_5c >= 0)
            glUniform4fv(this->f_5c, 1, (const float *)(e + 0x2b8));
        this->f_9 = 0;
    }

    if (this->f_20 >= 0)
        glEnableVertexAttribArray(this->f_20);
    if (this->f_24 >= 0)
        glEnableVertexAttribArray(this->f_24);
    if (this->f_28 >= 0)
        glEnableVertexAttribArray(this->f_28);
    if (this->f_2c >= 0)
        glEnableVertexAttribArray(this->f_2c);
    if (this->f_30 >= 0)
        glEnableVertexAttribArray(this->f_30);

    if (m->f_5c == 0) {
        // Client-side vertex arrays.
        if (this->f_20 >= 0)
            glVertexAttribPointer(this->f_20, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (this->f_24 >= 0)
            glVertexAttribPointer(this->f_24, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        if (this->f_28 >= 0)
            glVertexAttribPointer(this->f_28, 3, 0x1406, 0, 0, *(void **)(m + 0x10));
        if (this->f_2c >= 0)
            glVertexAttribPointer(this->f_2c, 3, 0x1406, 0, 0, *(void **)(m + 0x14));
        if (this->f_30 >= 0)
            glVertexAttribPointer(this->f_30, 3, 0x1406, 0, 0, *(void **)(m + 0x18));
    } else {
        // VBO path: bind each buffer and use zero offset.
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x60));
        glVertexAttribPointer(this->f_20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x68));
        glVertexAttribPointer(this->f_24, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x6c));
        glVertexAttribPointer(this->f_28, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x70));
        glVertexAttribPointer(this->f_2c, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x74));
        glVertexAttribPointer(this->f_30, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
