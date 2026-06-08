#include "class.h"

namespace AbyssEngine {

// AbyssEngine::GenericShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void GenericShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    if (this->f_34 >= 0)
        glUniformMatrix4fv(this->f_34, 1, 0, (const float *)(e + 0x104));
    if (this->f_38 >= 0)
        glUniformMatrix3fv(this->f_38, 1, 0, (const float *)(e + 0x204));

    if (this->f_9 != 0) {
        if (this->f_3c >= 0)
            glUniform3f(this->f_3c, e->f_330, e->f_334, e->f_338);
        if (this->f_40 >= 0)
            glUniform3f(this->f_40, e->f_34c, e->f_350, e->f_354);
        if (this->f_50 >= 0)
            glUniform4fv(this->f_50, 1, (const float *)(e + 0xd0));
        if (this->f_54 >= 0)
            glUniform3fv(this->f_54, 1, (const float *)(e + 0x2cc));
        if (this->f_58 >= 0)
            glUniform3fv(this->f_58, 1, (const float *)(e + 0x2fc));
        if (this->f_5c >= 0)
            glUniform3fv(this->f_5c, 1, (const float *)(e + 0x2e4));
        if (this->f_60 >= 0)
            glUniform1f(this->f_60, e->f_2c8);
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

    if (mesh->f_5c != 0) {
        void (*bindBuffer)(unsigned int, unsigned int) = glBindBuffer;
        void (*vertexAttribPointer)(int, int, unsigned int, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, m->f_60);
        vertexAttribPointer(this->f_20, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, m->f_68);
        vertexAttribPointer(this->f_24, 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, m->f_6c);
        vertexAttribPointer(this->f_28, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, m->f_70);
        vertexAttribPointer(this->f_2c, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, m->f_74);
        vertexAttribPointer(this->f_30, 3, 0x1406, 0, 0, 0);
        return;
    }

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
}

} // namespace AbyssEngine
