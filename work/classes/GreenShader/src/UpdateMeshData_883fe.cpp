#include "class.h"

namespace AbyssEngine {

void GreenShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->f_34 >= 0)
        glUniformMatrix4fv(this->f_34, 1, 0, (const float *)((char *)engine + 0x104));

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
        glBindBuffer(0x8892, mesh->f_60);
        glVertexAttribPointer(this->f_20, 3, 0x1406, 0, 0, 0);
    } else {
        if (this->f_20 < 0)
            return;
        glVertexAttribPointer(this->f_20, 3, 0x1406, 0, 0, (const void *)mesh->f_4);
    }
}

} // namespace AbyssEngine
