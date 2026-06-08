#include "class.h"

namespace AbyssEngine {

void TextureAlphaTestShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    uint8_t programIndex = g_TextureAlphaTestShader_programIndex;
    char *shader = (char *)this + programIndex * 4;

    glUniformMatrix4fv(*(int *)(shader + 0x34), 1, 0, (float *)((char *)engine + 0x104));

    if (this->f_9 != 0) {
        int location = *(int *)(shader + 0x3c);
        if (location >= 0) {
            glUniform4fv(location, 1, (float *)((char *)engine + 0xd0));
        }

        location = *(int *)(shader + 0x4c);
        if (location >= 0) {
            glUniform3fv(location, 1, Vector_cast_to_float((Vector *)((char *)engine + 0x3f0)));
        }

        location = *(int *)(shader + 0x5c);
        if (location >= 0) {
            glUniform1f(location, engine->f_3e8);
        }

        location = *(int *)(shader + 0x54);
        if (location >= 0) {
            glUniform1f(location, engine->f_3ec);
        }

        location = *(int *)(shader + 0x64);
        if (location >= 0) {
            glUniform1i(location, g_TextureAlphaTestShader_programIndex);
        }

        location = *(int *)(shader + 0x6c);
        if (location >= 0) {
            glUniform3f(location, engine->f_34c, engine->f_350, engine->f_354);
        }

        this->f_9 = 0;
    }

    if ((((uint32_t)mesh->f_0) << 30) < 0x80000000u) {
        return;
    }

    int *position = (int *)(shader + 0x24);
    int *texcoord = position + 2;
    glEnableVertexAttribArray(*position);
    glEnableVertexAttribArray(*texcoord);

    if (mesh->f_5c == 0) {
        glVertexAttribPointer(*position, 3, 0x1406, 0, 0, *(void **)((char *)mesh + 0x04));
        glVertexAttribPointer(*texcoord, 2, 0x1406, 0, 0, *(void **)((char *)mesh + 0x08));
    } else {
        glBindBuffer(0x8892, *(unsigned int *)((char *)mesh + 0x60));
        glVertexAttribPointer(*position, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)((char *)mesh + 0x68));
        glVertexAttribPointer(*texcoord, 2, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
