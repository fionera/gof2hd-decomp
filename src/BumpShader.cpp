#include "gof2/BumpShader.h"
#include "gof2/ShaderBaseStruct.h"
#include "gof2/String.h"


extern "C" void BumpShader_setSampler(int location, int unit);

// ---- SetInActive_8ba5c.cpp ----
namespace AbyssEngine {

void BumpShader::SetInActive()
{
    if (i32(this, 0x20) >= 0)
        glDisableVertexAttribArray(i32(this, 0x20));
    if (i32(this, 0x24) >= 0)
        glDisableVertexAttribArray(i32(this, 0x24));
    if (i32(this, 0x28) >= 0)
        glDisableVertexAttribArray(i32(this, 0x28));
}

} // namespace AbyssEngine

// ---- _BumpShader_8bd0c.cpp ----
void _ZN11AbyssEngine10BumpShaderD0Ev(BumpShader *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}

// ---- BumpShader_8b7f4.cpp ----
namespace AbyssEngine {

// AbyssEngine::BumpShader::BumpShader()
BumpShader::BumpShader()
{
    ShaderBaseStruct_ctor(this);

    // install vtable (+8 past RTTI/offset slots) and copy the type-info index static.
    *(void **)this = (void *)(BumpShader_vtable + 8);
    BumpShader_typeInfoDest = BumpShader_typeInfoSource;

    // name = String("BumpShader", false); this->name(0xc) = name; ~name.
    String name;
    ((String *)(&name))->ctor_char(BumpShader_name, false);
    ((String *)((String *)((char *)this + 0xc)))->assign(&name);
    ((String *)(&name))->dtor();
}

} // namespace AbyssEngine

// ---- Init_8b880.cpp ----
// Final indirect call in Init: an engine glUniform1i-style helper (DAT_001ab778).

namespace AbyssEngine {

// AbyssEngine::BumpShader::Init(AbyssEngine::Engine*)
//   Loads the program, then resolves every attribute/uniform location into this+0x20..0x80,
//   activates the program and binds sampler u(0x40)=0, sampler u(0x44)=7.
void BumpShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)(this))->ES2LoadProgram("BumpShader.vsh", "BumpShader.fsh");
    i32(this, 0x04) = program;

    i32(this, 0x20) = glGetAttribLocation(program, "a0");
    i32(this, 0x24) = glGetAttribLocation(i32(this, 0x04), "a1");
    i32(this, 0x28) = glGetAttribLocation(i32(this, 0x04), "a2");

    i32(this, 0x2c) = glGetUniformLocation(i32(this, 0x04), "u0");
    i32(this, 0x30) = glGetUniformLocation(i32(this, 0x04), "u1");
    i32(this, 0x34) = glGetUniformLocation(i32(this, 0x04), "u2");
    i32(this, 0x38) = glGetUniformLocation(i32(this, 0x04), "u3");
    i32(this, 0x3c) = glGetUniformLocation(i32(this, 0x04), "u4");
    i32(this, 0x40) = glGetUniformLocation(i32(this, 0x04), "u5");
    i32(this, 0x44) = glGetUniformLocation(i32(this, 0x04), "u6");
    i32(this, 0x48) = glGetUniformLocation(i32(this, 0x04), "u7");
    i32(this, 0x4c) = glGetUniformLocation(i32(this, 0x04), "u8");
    i32(this, 0x54) = glGetUniformLocation(i32(this, 0x04), "u9");
    i32(this, 0x5c) = glGetUniformLocation(i32(this, 0x04), "u10");
    i32(this, 0x50) = glGetUniformLocation(i32(this, 0x04), "u11");
    i32(this, 0x58) = glGetUniformLocation(i32(this, 0x04), "u12");
    i32(this, 0x60) = glGetUniformLocation(i32(this, 0x04), "u13");
    i32(this, 0x64) = glGetUniformLocation(i32(this, 0x04), "u14");
    i32(this, 0x68) = glGetUniformLocation(i32(this, 0x04), "u15");
    i32(this, 0x6c) = glGetUniformLocation(i32(this, 0x04), "u16");
    i32(this, 0x70) = glGetUniformLocation(i32(this, 0x04), "u17");
    i32(this, 0x74) = glGetUniformLocation(i32(this, 0x04), "u18");
    i32(this, 0x78) = glGetUniformLocation(i32(this, 0x04), "u19");
    i32(this, 0x7c) = glGetUniformLocation(i32(this, 0x04), "u20");
    i32(this, 0x80) = glGetUniformLocation(i32(this, 0x04), "u21");

    glUseProgram(i32(this, 0x04));
    glUniform1i(i32(this, 0x40), 0);
    BumpShader_setSampler(i32(this, 0x44), 7);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8ba88.cpp ----
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
