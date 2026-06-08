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

    if (this->f_2c >= 0)
        glUniformMatrix4fv(this->f_2c, 1, 0, (const float *)(e + 0x104));
    if (this->f_30 >= 0)
        glUniformMatrix3fv(this->f_30, 1, 0, (const float *)(e + 0x204));
    if (this->f_6c >= 0)
        glUniform1f(this->f_6c, *gBumpFloatA);
    if (this->f_70 >= 0)
        glUniform1f(this->f_70, *gBumpFloatB);

    if (this->f_9 != 0) {
        glUniform3f(this->f_34, *(float *)(e + 0x330), *(float *)(e + 0x334),
                    *(float *)(e + 0x338));
        if (this->f_3c >= 0)
            glUniform3f(this->f_3c, *(float *)(e + 0x34c), *(float *)(e + 0x350),
                        *(float *)(e + 0x354));
        if (this->f_48 >= 0)
            glUniform4fv(this->f_48, 1, (const float *)(e + 0xd0));
        if (this->f_4c >= 0)
            glUniform3fv(this->f_4c, 1, (const float *)(e + 0x2cc));
        if (this->f_54 >= 0)
            glUniform3fv(this->f_54, 1, (const float *)(e + 0x2fc));
        if (this->f_5c >= 0)
            glUniform3fv(this->f_5c, 1, (const float *)(e + 0x2e4));
        if (this->f_64 >= 0)
            glUniform1f(this->f_64, *(float *)(e + 0x2c8));
        if (this->f_68 >= 0)
            glUniform3fv(this->f_68, 1, (const float *)(e + 0x320));
        if (this->f_74 >= 0) {
            float *v = Vector_cast_to_float((Vector *)(e + 0x3f0));
            glUniform3fv(this->f_74, 1, v);
        }
        if (this->f_7c >= 0)
            glUniform1f(this->f_7c, *(float *)(e + 0x3e8));
        if (this->f_78 >= 0)
            glUniform1f(this->f_78, *(float *)(e + 0x3ec));
        if (this->f_80 >= 0)
            glUniform1i(this->f_80, *gBumpFlag);

        if (*(int *)(e + 0x32c) < 2) {
            // single-light path: zero the matrix uniforms, set the light dir.
            glUniform3f(this->f_50, 0, 0, 0);
            glUniform3f(this->f_58, 0, 0, 0);
            glUniform3f(this->f_60, 0, 0, 0);
            glUniform3f(this->f_38, *(float *)(e + 0x33c), *(float *)(e + 0x340),
                        *(float *)(e + 0x344));
        } else {
            glUniform3fv(this->f_50, 1, (const float *)(e + 0x2d8));
            glUniform3fv(this->f_58, 1, (const float *)(e + 0x308));
            glUniform3fv(this->f_60, 1, (const float *)(e + 0x2f0));
            glUniform3f(this->f_38, *(float *)(e + 0x33c), *(float *)(e + 0x340),
                        *(float *)(e + 0x344));
        }
        this->f_9 = 0;
    }

    if (this->f_20 >= 0)
        glEnableVertexAttribArray(this->f_20);
    if (this->f_24 >= 0)
        glEnableVertexAttribArray(this->f_24);
    if (this->f_28 >= 0)
        glEnableVertexAttribArray(this->f_28);

    if (mesh->f_5c == 0) {
        if (this->f_20 >= 0)
            glVertexAttribPointer(this->f_20, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (this->f_24 >= 0)
            glVertexAttribPointer(this->f_24, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        if (this->f_28 >= 0)
            glVertexAttribPointer(this->f_28, 3, 0x1406, 0, 0, *(void **)(m + 0x10));
    } else {
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x60));
        glVertexAttribPointer(this->f_20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x68));
        glVertexAttribPointer(this->f_24, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x6c));
        glVertexAttribPointer(this->f_28, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
