#include "class.h"

extern "C" void glUniform3fv(int location, int count, const float *value);
extern "C" float *_ZN11AbyssEngine6AEMath6VectorcvPfEv(void *self);

extern "C" float g_rimnGlobalA;
extern "C" float g_rimnGlobalB;
extern "C" float g_rimnGlobalC;
extern "C" unsigned char g_rimnByteGlobal;

namespace AbyssEngine {

void BumpRimCubeShader_new::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    void *volatile cookie = __stack_chk_guard;
    if (field_i32(this, 0x34) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x38) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x38), 1, 0, (char *)engine + 0x204);
    if (field_i32(this, 0x3c) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x3c), 1, 0, (char *)engine + 0x144);
    if (field_i32(this, 0x78) >= 0)
        glUniform1f(field_i32(this, 0x78), g_rimnGlobalA);
    if (field_i32(this, 0x7c) >= 0)
        glUniform1f(field_i32(this, 0x7c), g_rimnGlobalB);
    if (field_i32(this, 0x94) >= 0)
        glUniform1f(field_i32(this, 0x94), g_rimnGlobalC);

    if (field_u8(this, 0x9) != 0) {
        glUniform3f(field_i32(this, 0x40), field_f32(engine, 0x330),
                    field_f32(engine, 0x334), field_f32(engine, 0x338));
        if (field_i32(this, 0x48) >= 0)
            glUniform3f(field_i32(this, 0x48), field_f32(engine, 0x34c),
                        field_f32(engine, 0x350), field_f32(engine, 0x354));
        if (field_i32(this, 0x58) >= 0)
            glUniform4fv(field_i32(this, 0x58), 1, (float *)((char *)engine + 0xd0));
        float buf[3];
        if (field_i32(this, 0x5c) >= 0) {
            buf[0] = field_f32(engine, 0x2cc) + field_f32(engine, 0x2d8);
            buf[1] = field_f32(engine, 0x2d0) + field_f32(engine, 0x2dc);
            buf[2] = field_f32(engine, 0x2d4) + field_f32(engine, 0x2e0);
            glUniform3fv(field_i32(this, 0x5c), 1, buf);
        }
        if (field_i32(this, 0x60) >= 0)
            glUniform3fv(field_i32(this, 0x60), 1, (float *)((char *)engine + 0x2fc));
        if (field_i32(this, 0x68) >= 0)
            glUniform3fv(field_i32(this, 0x68), 1, (float *)((char *)engine + 0x2e4));
        if (field_i32(this, 0x70) >= 0)
            glUniform1f(field_i32(this, 0x70), field_f32(engine, 0x2c8));
        if (field_i32(this, 0x74) >= 0)
            glUniform3fv(field_i32(this, 0x74), 1, (float *)((char *)engine + 800));
        int loc84 = field_i32(this, 0x84);
        if (loc84 >= 0) {
            float *v = _ZN11AbyssEngine6AEMath6VectorcvPfEv((char *)engine + 0x3f0);
            glUniform3fv(loc84, 1, v);
        }
        if (field_i32(this, 0x8c) >= 0)
            glUniform1f(field_i32(this, 0x8c), field_f32(engine, 1000));
        if (field_i32(this, 0x88) >= 0)
            glUniform1f(field_i32(this, 0x88), field_f32(engine, 0x3ec));
        if (field_i32(this, 0x90) >= 0)
            glUniform1i(field_i32(this, 0x90), g_rimnByteGlobal);
        if (field_i32(this, 0x80) >= 0) {
            float v = 0.0f;
            int *m30 = (int *)field_ptr(mesh, 0x30);
            if (m30 != 0) {
                v = 1.0f;
                if (m30[9] == 0)
                    v = 0.0f;
            }
            glUniform1f(field_i32(this, 0x80), v);
        }
        int loc5c = field_i32(this, 0x5c);
        if (field_i32(engine, 0x32c) >= 2) {
            if (loc5c >= 0) {
                buf[0] = field_f32(engine, 0x2cc) + field_f32(engine, 0x2d8);
                buf[1] = field_f32(engine, 0x2d0) + field_f32(engine, 0x2dc);
                buf[2] = field_f32(engine, 0x2d4) + field_f32(engine, 0x2e0);
                glUniform3fv(loc5c, 1, buf);
            }
            glUniform3fv(field_i32(this, 0x64), 1, (float *)((char *)engine + 0x308));
            glUniform3fv(field_i32(this, 0x6c), 1, (float *)((char *)engine + 0x2f0));
            glUniform3f(field_i32(this, 0x44), field_f32(engine, 0x33c),
                        field_f32(engine, 0x340), field_f32(engine, 0x344));
        } else {
            glUniform3f(loc5c, 0, 0, 0);
            glUniform3f(field_i32(this, 0x64), 0, 0, 0);
            glUniform3f(field_i32(this, 0x6c), 0, 0, 0);
            glUniform3f(field_i32(this, 0x44), field_f32(engine, 0x33c),
                        field_f32(engine, 0x340), field_f32(engine, 0x344));
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
        glBindBuffer(0x8892, field_i32(mesh, 0x60));
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x68));
        glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x6c));
        glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x70));
        glVertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_i32(mesh, 0x74));
        glVertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, 0);
    }
    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta != 0)
        __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
