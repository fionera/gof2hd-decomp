#include "class.h"

namespace AbyssEngine {

void TextureShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    uint8_t slotIndex = g_TextureShader_activeSlot;
    char *shader = bytes(this) + slotIndex * 4;
    char *meshBytes = bytes(mesh);
    char *engineBytes = bytes(engine);

    glUniformMatrix4fv(*(int *)(shader + 0x34), 1, 0, engineBytes + 0x104);

    int location = *(int *)(shader + 0x4c);
    if (location >= 0) {
        glUniformMatrix4fv(location, 1, 0, engineBytes + 0x1c4);
    }

    location = *(int *)(shader + 0x84);
    if (location >= 0) {
        glUniform1f(location, 1.0f - *(float *)(meshBytes + 0x1c));
    }

    if (*(uint8_t *)(bytes(this) + 0x09) != 0) {
        glUniform4fv(*(int *)(shader + 0x3c), 1, (float *)(engineBytes + 0xd0));

        location = *(int *)(shader + 0x54);
        if (location >= 0) {
            glUniform1i(location, *(uint8_t *)(meshBytes + 0x85));
        }

        location = *(int *)(shader + 0x5c);
        if (location >= 0) {
            glUniform3fv(location, 1, Vector_to_float((Vector *)(engineBytes + 0x3f0)));
        }

        location = *(int *)(shader + 0x6c);
        if (location >= 0) {
            glUniform1f(location, *(float *)(engineBytes + 0x3e8));
        }

        location = *(int *)(shader + 0x64);
        if (location >= 0) {
            glUniform1f(location, *(float *)(engineBytes + 0x3ec));
        }

        location = *(int *)(shader + 0x74);
        if (location >= 0) {
            glUniform1i(location, g_TextureShader_activeSlot);
        }

        location = *(int *)(shader + 0x7c);
        if (location >= 0) {
            glUniform3f(location, *(float *)(engineBytes + 0x34c), *(float *)(engineBytes + 0x350),
                        *(float *)(engineBytes + 0x354));
        }

        *(uint8_t *)(bytes(this) + 0x09) = 0;
    }

    if ((int)(*(uint8_t *)meshBytes << 30) < 0) {
        shader += 0x24;
        char *position = shader;
        glEnableVertexAttribArray(*(uint32_t *)position);

        shader += 8;
        glEnableVertexAttribArray(*(uint32_t *)shader);

        if (*(uint8_t *)(meshBytes + 0x5c) == 0) {
            glVertexAttribPointer(*(uint32_t *)position, 3, 0x1406, 0, 0, *(void **)(meshBytes + 0x04));
            glVertexAttribPointer(*(uint32_t *)shader, 2, 0x1406, 0, 0, *(void **)(meshBytes + 0x08));
        } else {
            glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x60));
            glVertexAttribPointer(*(uint32_t *)position, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x68));
            glVertexAttribPointer(*(uint32_t *)shader, 2, 0x1406, 0, 0, 0);
        }
    }
}

} // namespace AbyssEngine
