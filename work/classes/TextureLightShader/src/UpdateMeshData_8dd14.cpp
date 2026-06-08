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
    glUniformMatrix4fv(this->f_2c, 1, 0, ef(engine, 0x104));
    if (this->f_68 >= 0)
        glUniformMatrix4fv(this->f_68, 1, 0, ef(engine, 0x1c4));
    glUniformMatrix3fv(this->f_34, 1, 0, ef(engine, 0x204));
    if (this->f_30 >= 0)
        glUniformMatrix4fv(this->f_30, 1, 0, ef(engine, 0x144));

    if (this->f_9 != 0) {
        if (this->f_6c >= 0)
            glUniform1i(this->f_6c, *(uint8_t *)((char *)mesh + 0x85));
        glUniform4fv(this->f_40, 1, ef(engine, 0xd0));

        glUniform3fv(this->f_48, 1, ef(engine, 0x2cc));
        glUniform3fv(this->f_50, 1, ef(engine, 0x2fc));
        glUniform3fv(this->f_58, 1, ef(engine, 0x2e4));

        glUniform4f(this->f_38, eff(engine, 0x330), eff(engine, 0x334),
                    eff(engine, 0x338), eff(engine, 0x378));

        if (ei(engine, 0x32c) < 2) {
            glUniform3f(this->f_4c, 0, 0, 0);
            glUniform3f(this->f_54, 0, 0, 0);
            glUniform3f(this->f_5c, 0, 0, 0);
        } else {
            glUniform3fv(this->f_4c, 1, ef(engine, 0x2d8));
            glUniform3fv(this->f_54, 1, ef(engine, 0x308));
            glUniform3fv(this->f_5c, 1, ef(engine, 0x2f0));
        }

        glUniform4f(this->f_3c, eff(engine, 0x33c), eff(engine, 0x340),
                    eff(engine, 0x344), eff(engine, 0x37c));
        glUniform1f(this->f_60, eff(engine, 0x2c8));
        if (this->f_64 >= 0)
            glUniform3f(this->f_64, eff(engine, 0x34c), eff(engine, 0x350),
                        eff(engine, 0x354));
        this->f_9 = 0;
    }

    glEnableVertexAttribArray(this->f_20);
    glEnableVertexAttribArray(this->f_24);
    glEnableVertexAttribArray(this->f_28);

    if (*(uint8_t *)((char *)mesh + 0x5c) != 0) {
        glBindBuffer(0x8892, *(int *)((char *)mesh + 0x60));
        glVertexAttribPointer(this->f_20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)((char *)mesh + 0x68));
        glVertexAttribPointer(this->f_24, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)((char *)mesh + 0x6c));
        glVertexAttribPointer(this->f_28, 3, 0x1406, 0, 0, 0);
        return;
    }

    glVertexAttribPointer(this->f_20, 3, 0x1406, 0, 0,
                          *(const void **)((char *)mesh + 0x4));
    glVertexAttribPointer(this->f_24, 2, 0x1406, 0, 0,
                          *(const void **)((char *)mesh + 0x8));
    glVertexAttribPointer(this->f_28, 3, 0x1406, 0, 0,
                          *(const void **)((char *)mesh + 0x10));
}

} // namespace AbyssEngine
