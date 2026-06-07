#include "class.h"

extern "C" float *g_bsv3_floatA;
extern "C" float *g_bsv3_floatB;

namespace AbyssEngine {

void BumpShaderV3::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x34) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x38) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x38), 1, 0, (char *)engine + 0x204);
    if (field_i32(this, 0x78) >= 0)
        glUniform1f(field_i32(this, 0x78), *g_bsv3_floatA);
    if (field_i32(this, 0x7c) >= 0)
        glUniform1f(field_i32(this, 0x7c), *g_bsv3_floatB);

    if (field_u8(this, 0x9) != 0) {
        glUniform3f(field_i32(this, 0x3c), field_f32(engine, 0x330), field_f32(engine, 0x334),
                    field_f32(engine, 0x338));
        if (field_i32(this, 0x44) >= 0)
            glUniform3f(field_i32(this, 0x44), field_f32(engine, 0x34c), field_f32(engine, 0x350),
                        field_f32(engine, 0x354));
        if (field_i32(this, 0x54) >= 0)
            glUniform4fv(field_i32(this, 0x54), 1, (float *)((char *)engine + 0xd0));
        if (field_i32(this, 0x58) >= 0)
            glUniform3fv(field_i32(this, 0x58), 1, (float *)((char *)engine + 0x2cc));
        if (field_i32(this, 0x60) >= 0)
            glUniform3fv(field_i32(this, 0x60), 1, (float *)((char *)engine + 0x2fc));
        if (field_i32(this, 0x68) >= 0)
            glUniform3fv(field_i32(this, 0x68), 1, (float *)((char *)engine + 0x2e4));
        if (field_i32(this, 0x70) >= 0)
            glUniform1f(field_i32(this, 0x70), field_f32(engine, 0x2c8));
        if (field_i32(this, 0x74) >= 0)
            glUniform3fv(field_i32(this, 0x74), 1, (float *)((char *)engine + 0x320));
        if (field_i32(this, 0x80) >= 0) {
            float w = 0.85f;
            if (*(int *)(*(int *)((char *)mesh + 0x30) + 0x24) != 0)
                w = 1.0f;
            glUniform1f(field_i32(this, 0x80), w);
        }
        if (field_i32(engine, 0x32c) >= 2) {
            void (*uniform3fv)(int, int, const float *) = glUniform3fv;
            uniform3fv(field_i32(this, 0x5c), 1, (float *)((char *)engine + 0x2d8));
            uniform3fv(field_i32(this, 0x64), 1, (float *)((char *)engine + 0x308));
            uniform3fv(field_i32(this, 0x6c), 1, (float *)((char *)engine + 0x2f0));
            glUniform3f(field_i32(this, 0x40), field_f32(engine, 0x33c), field_f32(engine, 0x340),
                        field_f32(engine, 0x344));
        } else {
            void (*uniform3f)(int, float, float, float) = glUniform3f;
            uniform3f(field_i32(this, 0x5c), 0, 0, 0);
            uniform3f(field_i32(this, 0x64), 0, 0, 0);
            uniform3f(field_i32(this, 0x6c), 0, 0, 0);
            uniform3f(field_i32(this, 0x40), field_f32(engine, 0x33c), field_f32(engine, 0x340),
                      field_f32(engine, 0x344));
        }
        field_u8(this, 0x9) = 0;
    }

    if (field_i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x20));
    if (field_i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x24));
    if (field_i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x28));
    if (field_i32(this, 0x2c) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x2c));
    if (field_i32(this, 0x30) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x30));

    if (field_u8(mesh, 0x5c) == 0) {
        if (field_i32(this, 0x20) >= 0)
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (field_i32(this, 0x24) >= 0)
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        if (field_i32(this, 0x28) >= 0)
            glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
        if (field_i32(this, 0x2c) >= 0)
            glVertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
        if (field_i32(this, 0x30) >= 0)
            glVertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
    } else {
        void (*bindBuffer)(uint32_t, uint32_t) = glBindBuffer;
        void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, field_i32(mesh, 0x60));
        vertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x68));
        vertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x6c));
        vertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x70));
        vertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, field_i32(mesh, 0x74));
        vertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
