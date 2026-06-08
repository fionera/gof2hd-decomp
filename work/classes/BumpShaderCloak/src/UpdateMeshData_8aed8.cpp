#include "class.h"

extern "C" void glUniform2f(int location, float x, float y);
extern "C" void glUniformMatrix3fv(int location, int count, uint8_t transpose, const void *value);
extern "C" int _ZN11AbyssEngine6Engine15GetDisplayWidthEv(void *self);
extern "C" int _ZN11AbyssEngine6Engine16GetDisplayHeightEv(void *self);
extern "C" void _ZN11AbyssEngine6Engine18ActivateRefractFBOEv(void *self);

namespace AbyssEngine {

// AbyssEngine::BumpShaderCloak::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void BumpShaderCloak::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *m = (char *)mesh;
    char *e = (char *)engine;

    if (field_i32(this, 0x34) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, e + 0x104);
    if (field_i32(this, 0x38) >= 0)
        glUniformMatrix3fv(field_i32(this, 0x38), 1, 0, e + 0x204);

    if (field_u8(this, 0x9)) {
        glUniform3f(field_i32(this, 0x3c), field_f32(e, 0x330), field_f32(e, 0x334), field_f32(e, 0x338));
        if (field_i32(this, 0x44) >= 0)
            glUniform3f(field_i32(this, 0x44), field_f32(e, 0x34c), field_f32(e, 0x350), field_f32(e, 0x354));
        if (field_i32(this, 0x58) >= 0)
            glUniform4fv(field_i32(this, 0x58), 1, (const float *)(e + 0xd0));
        if (field_i32(this, 0x5c) >= 0)
            glUniform3fv(field_i32(this, 0x5c), 1, (const float *)(e + 0x2cc));
        if (field_i32(this, 0x64) >= 0)
            glUniform3fv(field_i32(this, 0x64), 1, (const float *)(e + 0x2fc));
        if (field_i32(this, 0x6c) >= 0)
            glUniform3fv(field_i32(this, 0x6c), 1, (const float *)(e + 0x2e4));
        if (field_i32(this, 0x74) >= 0)
            glUniform1f(field_i32(this, 0x74), field_f32(e, 0x2c8));
        if (field_i32(this, 0x78) >= 0)
            glUniform3fv(field_i32(this, 0x78), 1, (const float *)(e + 0x320));

        int viewportLoc = field_i32(this, 0x8c);
        if (viewportLoc >= 0) {
            // The render-target's mode (at [[engine+0x30]+0x30]) picks portrait vs landscape.
            float invW, invH;
            if (*(int *)(*(char **)(e + 0x30) + 0x30) == 2) {
                invW = 1.0f / (float)_ZN11AbyssEngine6Engine15GetDisplayWidthEv(engine);
                invH = 1.0f / (float)_ZN11AbyssEngine6Engine16GetDisplayHeightEv(engine);
            } else {
                invW = 1.0f / (float)_ZN11AbyssEngine6Engine16GetDisplayHeightEv(engine);
                invH = 1.0f / (float)_ZN11AbyssEngine6Engine15GetDisplayWidthEv(engine);
            }
            glUniform2f(viewportLoc, invW, invH);
        }

        glActiveTexture(0x84c7);
        _ZN11AbyssEngine6Engine18ActivateRefractFBOEv(engine);

        if (field_i32(this, 0x80) >= 0)
            glUniform1f(field_i32(this, 0x80), field_f32(m, 0x1c));
        if (field_i32(this, 0x84) >= 0)
            glUniform1f(field_i32(this, 0x84), field_f32(m, 0x20));

        if (field_i32(e, 0x32c) < 2) {
            glUniform3f(field_i32(this, 0x60), 0.0f, 0.0f, 0.0f);
            glUniform3f(field_i32(this, 0x68), 0.0f, 0.0f, 0.0f);
            glUniform3f(field_i32(this, 0x70), 0.0f, 0.0f, 0.0f);
            glUniform3f(field_i32(this, 0x40), field_f32(e, 0x33c), field_f32(e, 0x340), field_f32(e, 0x344));
        } else {
            glUniform3fv(field_i32(this, 0x60), 1, (const float *)(e + 0x2d8));
            glUniform3fv(field_i32(this, 0x68), 1, (const float *)(e + 0x308));
            glUniform3fv(field_i32(this, 0x70), 1, (const float *)(e + 0x2f0));
            glUniform3f(field_i32(this, 0x40), field_f32(e, 0x33c), field_f32(e, 0x340), field_f32(e, 0x344));
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

    if (field_u8(m, 0x5c) == 0) {
        if (field_i32(this, 0x20) >= 0)
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (field_i32(this, 0x24) >= 0)
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        if (field_i32(this, 0x28) >= 0)
            glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, *(void **)(m + 0x10));
        if (field_i32(this, 0x2c) >= 0)
            glVertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, *(void **)(m + 0x14));
        if (field_i32(this, 0x30) >= 0)
            glVertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, *(void **)(m + 0x18));
    } else {
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x60));
        glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x68));
        glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x6c));
        glVertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x70));
        glVertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(unsigned int *)(m + 0x74));
        glVertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
