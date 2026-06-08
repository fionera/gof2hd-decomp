#include "class.h"

// PC-relative pointer-to-pointer globals holding single floats / a byte (engine timers and
// a feature flag) read by UpdateMeshData.
extern float *const gBumpFloatA __attribute__((visibility("hidden")));   // DAT_0009bcf0
extern float *const gBumpFloatB __attribute__((visibility("hidden")));   // DAT_0009bcf4
extern uint8_t *const gBumpFlag __attribute__((visibility("hidden")));   // DAT_0009bcf8

namespace AbyssEngine {

// AbyssEngine::BumpShader::UpdateMeshData(AbyssEngine::Mesh* mesh, AbyssEngine::Engine* ctx)
//   Pushes the per-frame matrices/lighting uniforms (only the first time per frame, gated by
//   this[0x9]) and binds the mesh's vertex attribute streams.
void BumpShader::UpdateMeshData(Mesh *mesh, Engine *ctx)
{
    char *e = (char *)ctx;
    char *m = (char *)mesh;

    if (i32(this, 0x2c) >= 0)
        glUniformMatrix4fv(i32(this, 0x2c), 1, 0, (const float *)(e + 0x104));
    if (i32(this, 0x30) >= 0)
        glUniformMatrix3fv(i32(this, 0x30), 1, 0, (const float *)(e + 0x204));
    if (i32(this, 0x6c) >= 0)
        glUniform1f(i32(this, 0x6c), *gBumpFloatA);
    if (i32(this, 0x70) >= 0)
        glUniform1f(i32(this, 0x70), *gBumpFloatB);

    if (u8(this, 0x9) != 0) {
        glUniform3f(i32(this, 0x34), *(float *)(e + 0x330), *(float *)(e + 0x334),
                    *(float *)(e + 0x338));
        if (i32(this, 0x3c) >= 0)
            glUniform3f(i32(this, 0x3c), *(float *)(e + 0x34c), *(float *)(e + 0x350),
                        *(float *)(e + 0x354));
        if (i32(this, 0x48) >= 0)
            glUniform4fv(i32(this, 0x48), 1, (const float *)(e + 0xd0));
        if (i32(this, 0x4c) >= 0)
            glUniform3fv(i32(this, 0x4c), 1, (const float *)(e + 0x2cc));
        if (i32(this, 0x54) >= 0)
            glUniform3fv(i32(this, 0x54), 1, (const float *)(e + 0x2fc));
        if (i32(this, 0x5c) >= 0)
            glUniform3fv(i32(this, 0x5c), 1, (const float *)(e + 0x2e4));
        if (i32(this, 0x64) >= 0)
            glUniform1f(i32(this, 0x64), *(float *)(e + 0x2c8));
        if (i32(this, 0x68) >= 0)
            glUniform3fv(i32(this, 0x68), 1, (const float *)(e + 0x320));
        if (i32(this, 0x74) >= 0) {
            float *v = Vector_cast_to_float((Vector *)(e + 0x3f0));
            glUniform3fv(i32(this, 0x74), 1, v);
        }
        if (i32(this, 0x7c) >= 0)
            glUniform1f(i32(this, 0x7c), *(float *)(e + 0x3e8));
        if (i32(this, 0x78) >= 0)
            glUniform1f(i32(this, 0x78), *(float *)(e + 0x3ec));
        if (i32(this, 0x80) >= 0)
            glUniform1i(i32(this, 0x80), *gBumpFlag);

        if (*(int *)(e + 0x32c) < 2) {
            // single-light path: zero the matrix uniforms, set the light dir.
            glUniform3f(i32(this, 0x50), 0, 0, 0);
            glUniform3f(i32(this, 0x58), 0, 0, 0);
            glUniform3f(i32(this, 0x60), 0, 0, 0);
            glUniform3f(i32(this, 0x38), *(float *)(e + 0x33c), *(float *)(e + 0x340),
                        *(float *)(e + 0x344));
        } else {
            glUniform3fv(i32(this, 0x50), 1, (const float *)(e + 0x2d8));
            glUniform3fv(i32(this, 0x58), 1, (const float *)(e + 0x308));
            glUniform3fv(i32(this, 0x60), 1, (const float *)(e + 0x2f0));
            glUniform3f(i32(this, 0x38), *(float *)(e + 0x33c), *(float *)(e + 0x340),
                        *(float *)(e + 0x344));
        }
        u8(this, 0x9) = 0;
    }

    if (i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(i32(this, 0x20));
    if (i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(i32(this, 0x24));
    if (i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(i32(this, 0x28));

    if (u8(mesh, 0x5c) == 0) {
        if (i32(this, 0x20) >= 0)
            glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (i32(this, 0x24) >= 0)
            glVertexAttribPointer(i32(this, 0x24), 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        if (i32(this, 0x28) >= 0)
            glVertexAttribPointer(i32(this, 0x28), 3, 0x1406, 0, 0, *(void **)(m + 0x10));
    } else {
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x60));
        glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x68));
        glVertexAttribPointer(i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x6c));
        glVertexAttribPointer(i32(this, 0x28), 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
