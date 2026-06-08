#include "class.h"

namespace AbyssEngine {

// Helpers to view the Engine/Mesh objects at raw byte offsets.
static inline const float *ef(Engine *e, unsigned off)
{
    return (const float *)((char *)e + off);
}
static inline int ei(Engine *e, unsigned off)
{
    return *(int *)((char *)e + off);
}
static inline float eff(Engine *e, unsigned off)
{
    return *(float *)((char *)e + off);
}

// TextureLightShader::UpdateMeshData(Mesh*, Engine*)
void TextureLightShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(i32(this, 0x2c), 1, 0, ef(engine, 0x104));
    if (i32(this, 0x68) >= 0)
        glUniformMatrix4fv(i32(this, 0x68), 1, 0, ef(engine, 0x1c4));
    glUniformMatrix3fv(i32(this, 0x34), 1, 0, ef(engine, 0x204));
    if (i32(this, 0x30) >= 0)
        glUniformMatrix4fv(i32(this, 0x30), 1, 0, ef(engine, 0x144));

    if (u8(this, 0x9) != 0) {
        if (i32(this, 0x6c) >= 0)
            glUniform1i(i32(this, 0x6c), *(uint8_t *)((char *)mesh + 0x85));
        glUniform4fv(i32(this, 0x40), 1, ef(engine, 0xd0));

        glUniform3fv(i32(this, 0x48), 1, ef(engine, 0x2cc));
        glUniform3fv(i32(this, 0x50), 1, ef(engine, 0x2fc));
        glUniform3fv(i32(this, 0x58), 1, ef(engine, 0x2e4));

        glUniform4f(i32(this, 0x38), eff(engine, 0x330), eff(engine, 0x334),
                    eff(engine, 0x338), eff(engine, 0x378));

        if (ei(engine, 0x32c) < 2) {
            glUniform3f(i32(this, 0x4c), 0, 0, 0);
            glUniform3f(i32(this, 0x54), 0, 0, 0);
            glUniform3f(i32(this, 0x5c), 0, 0, 0);
        } else {
            glUniform3fv(i32(this, 0x4c), 1, ef(engine, 0x2d8));
            glUniform3fv(i32(this, 0x54), 1, ef(engine, 0x308));
            glUniform3fv(i32(this, 0x5c), 1, ef(engine, 0x2f0));
        }

        glUniform4f(i32(this, 0x3c), eff(engine, 0x33c), eff(engine, 0x340),
                    eff(engine, 0x344), eff(engine, 0x37c));
        glUniform1f(i32(this, 0x60), eff(engine, 0x2c8));
        if (i32(this, 0x64) >= 0)
            glUniform3f(i32(this, 0x64), eff(engine, 0x34c), eff(engine, 0x350),
                        eff(engine, 0x354));
        u8(this, 0x9) = 0;
    }

    glEnableVertexAttribArray(i32(this, 0x20));
    glEnableVertexAttribArray(i32(this, 0x24));
    glEnableVertexAttribArray(i32(this, 0x28));

    if (*(uint8_t *)((char *)mesh + 0x5c) != 0) {
        glBindBuffer(0x8892, *(int *)((char *)mesh + 0x60));
        glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)((char *)mesh + 0x68));
        glVertexAttribPointer(i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)((char *)mesh + 0x6c));
        glVertexAttribPointer(i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        return;
    }

    glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0,
                          *(const void **)((char *)mesh + 0x4));
    glVertexAttribPointer(i32(this, 0x24), 2, 0x1406, 0, 0,
                          *(const void **)((char *)mesh + 0x8));
    glVertexAttribPointer(i32(this, 0x28), 3, 0x1406, 0, 0,
                          *(const void **)((char *)mesh + 0x10));
}

} // namespace AbyssEngine
