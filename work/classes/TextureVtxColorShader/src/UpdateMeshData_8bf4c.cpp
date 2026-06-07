#include "class.h"

namespace AbyssEngine {

void TextureVtxColorShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    uint8_t *fogEnabled = (uint8_t *)&Engine::fogEnabled;
    uint8_t fogIndex = *fogEnabled;
    char *entry = (char *)this + fogIndex * 4;

    glUniformMatrix4fv(*(int *)(entry + 0x3c), 1, 0, (float *)((char *)engine + 0x104));

    int location = *(int *)(entry + 0x5c);
    if (location >= 0) {
        glUniformMatrix4fv(location, 1, 0, (float *)((char *)engine + 0x1c4));
    }

    location = *(int *)(entry + 0x8c);
    if (location >= 0) {
        glUniform1f(location, 1.0f - *(float *)((char *)mesh + 0x1c));
    }

    if (field_u8(this, 0x09) != 0) {
        glUniform4fv(*(int *)(entry + 0x44), 1, (float *)((char *)engine + 0xd0));

        location = *(int *)(entry + 0x54);
        if (location >= 0) {
            glUniform1i(location, *(uint8_t *)((char *)mesh + 0x85));
        }

        location = *(int *)(entry + 0x64);
        if (location >= 0) {
            AEMath::Vector *eye = (AEMath::Vector *)((char *)engine + 0x3f0);
            glUniform3fv(location, 1, eye->operator float *());
        }

        location = *(int *)(entry + 0x74);
        if (location >= 0) {
            glUniform1f(location, field_f32(engine, 0x3e8));
        }

        location = *(int *)(entry + 0x6c);
        if (location >= 0) {
            glUniform1f(location, field_f32(engine, 0x3ec));
        }

        location = *(int *)(entry + 0x7c);
        if (location >= 0) {
            glUniform1i(location, *fogEnabled);
        }

        location = *(int *)(entry + 0x84);
        if (location >= 0) {
            glUniform3f(
                location,
                field_f32(engine, 0x34c),
                field_f32(engine, 0x350),
                field_f32(engine, 0x354));
        }

        field_u8(this, 0x09) = 0;
    }

    void (*enableVertexAttribArray)(uint32_t) = glEnableVertexAttribArray;
    char *position = entry;
    enableVertexAttribArray(*(uint32_t *)(position += 0x24));
    char *texCoord = entry;
    enableVertexAttribArray(*(uint32_t *)(texCoord += 0x2c));
    char *vertexColor = entry;
    enableVertexAttribArray(*(uint32_t *)(vertexColor += 0x34));

    if (*(uint8_t *)((char *)mesh + 0x5c) != 0) {
        void (*bindBuffer)(uint32_t, uint32_t) = glBindBuffer;
        void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, *(uint32_t *)((char *)mesh + 0x60));
        vertexAttribPointer(*(uint32_t *)position, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, *(uint32_t *)((char *)mesh + 0x68));
        vertexAttribPointer(*(uint32_t *)texCoord, 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, *(uint32_t *)((char *)mesh + 0x78));
        vertexAttribPointer(*(uint32_t *)vertexColor, 4, 0x1406, 0, 0, 0);
        return;
    }

    void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
        glVertexAttribPointer;
    vertexAttribPointer(*(uint32_t *)position, 3, 0x1406, 0, 0, field_ptr(mesh, 0x04));
    vertexAttribPointer(*(uint32_t *)texCoord, 2, 0x1406, 0, 0, field_ptr(mesh, 0x08));
    return vertexAttribPointer(*(uint32_t *)vertexColor, 4, 0x1406, 0, 0, field_ptr(mesh, 0x0c));
}

} // namespace AbyssEngine
