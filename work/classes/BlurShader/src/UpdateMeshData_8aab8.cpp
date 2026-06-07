#include "class.h"

extern "C" void glUniformMatrix4fv(int location, int count, unsigned char transpose,
                                   const void *value);
extern "C" void glUniform2f(int location, float x, float y);
extern "C" void glUniform1f(int location, float value);
extern "C" void glEnableVertexAttribArray(unsigned int index);
extern "C" void glBindBuffer(unsigned int target, unsigned int buffer);
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type,
                                      unsigned char normalized, int stride, const void *pointer);
extern "C" int Engine_GetDisplayWidth(Engine *self);
extern "C" int Engine_GetDisplayHeight(Engine *self);

namespace AbyssEngine {

void BlurShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    int matrixLocation = *(int *)((char *)this + 0x2c);
    if (matrixLocation >= 0) {
        glUniformMatrix4fv(matrixLocation, 1, 0, (char *)engine + 0x104);
    }

    int texelLocation = *(int *)((char *)this + 0x38);
    if (texelLocation >= 0) {
        float width;
        float height;
        float one = 1.0f;
        if (*(int *)(**(char ***)((char *)engine + 0x30) + 0x30) == 2) {
            width = (float)Engine_GetDisplayWidth(engine);
            height = (float)Engine_GetDisplayHeight(engine);
        } else {
            width = (float)Engine_GetDisplayHeight(engine);
            height = (float)Engine_GetDisplayWidth(engine);
        }
        glUniform2f(texelLocation, one / width, one / height);
    }

    int centerLocation = *(int *)((char *)this + 0x44);
    if (centerLocation >= 0) {
        glUniform2f(centerLocation, 0.5f, 0.5f);
    }

    int strengthLocation = *(int *)((char *)this + 0x40);
    if (strengthLocation >= 0) {
        glUniform1f(strengthLocation, *(float *)((char *)this + 0x58));
    }

    int amountLocation = *(int *)((char *)this + 0x3c);
    if (amountLocation >= 0) {
        glUniform1f(amountLocation, *(float *)((char *)this + 0x5c));
    }

    if (*(uint8_t *)((char *)this + 0x9) != 0) {
        *(uint8_t *)((char *)this + 0x9) = 0;
    }

    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x48));
    glEnableVertexAttribArray(*(unsigned int *)((char *)this + 0x50));

    const void *texPtr;
    if (*(uint8_t *)((char *)mesh + 0x5c) == 0) {
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x48), 3, 0x1406, 0, 0,
                              *(void **)((char *)mesh + 0x4));
        unsigned int texCoord = *(unsigned int *)((char *)this + 0x50);
        texPtr = *(void **)((char *)mesh + 0x8);
        return glVertexAttribPointer(texCoord, 2, 0x1406, 0, 0, texPtr);
    } else {
        glBindBuffer(0x8892, *(unsigned int *)((char *)mesh + 0x60));
        glVertexAttribPointer(*(unsigned int *)((char *)this + 0x48), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)((char *)mesh + 0x68));
        unsigned int texCoord = *(unsigned int *)((char *)this + 0x50);
        texPtr = 0;
        return glVertexAttribPointer(texCoord, 2, 0x1406, 0, 0, texPtr);
    }
}

} // namespace AbyssEngine
