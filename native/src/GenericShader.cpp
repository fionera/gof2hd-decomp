#include "gof2/GenericShader.h"




// ---- Init_8e7dc.cpp ----
namespace AbyssEngine {

void GenericShader::Init(Engine *)
{
    int program = ShaderBaseStruct_ES2LoadProgram(this, "GenericShader.vsh", "GenericShader.fsh");
    i32(this, 0x04) = program;

    i32(this, 0x20) = glGetAttribLocation(program, "a0");
    i32(this, 0x24) = glGetAttribLocation(i32(this, 0x04), "a1");
    i32(this, 0x28) = glGetAttribLocation(i32(this, 0x04), "a2");
    i32(this, 0x2c) = glGetAttribLocation(i32(this, 0x04), "a3");
    i32(this, 0x30) = glGetAttribLocation(i32(this, 0x04), "a4");

    i32(this, 0x34) = glGetUniformLocation(i32(this, 0x04), "u0");
    i32(this, 0x38) = glGetUniformLocation(i32(this, 0x04), "u1");
    i32(this, 0x3c) = glGetUniformLocation(i32(this, 0x04), "u2");
    i32(this, 0x40) = glGetUniformLocation(i32(this, 0x04), "u3");
    i32(this, 0x44) = glGetUniformLocation(i32(this, 0x04), "u4");
    i32(this, 0x48) = glGetUniformLocation(i32(this, 0x04), "u5");
    i32(this, 0x4c) = glGetUniformLocation(i32(this, 0x04), "u6");
    i32(this, 0x50) = glGetUniformLocation(i32(this, 0x04), "u7");
    i32(this, 0x54) = glGetUniformLocation(i32(this, 0x04), "u8");
    i32(this, 0x58) = glGetUniformLocation(i32(this, 0x04), "u9");
    i32(this, 0x5c) = glGetUniformLocation(i32(this, 0x04), "u10");
    i32(this, 0x60) = glGetUniformLocation(i32(this, 0x04), "u11");

    glUseProgram(i32(this, 0x04));
    for (int i = 0; i != 3; i++) {
        if (i32(this, 0x44 + i * 4) >= 0)
            glUniform1i(i32(this, 0x44 + i * 4), i);
    }
}

} // namespace AbyssEngine

// ---- _GenericShader_8eb90.cpp ----
extern "C" void _ZN11AbyssEngine13GenericShaderD0Ev(GenericShader *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}

// ---- SetInActive_8e940.cpp ----
namespace AbyssEngine {

void GenericShader::SetInActive()
{
    if (i32(this, 0x20) >= 0)
        glDisableVertexAttribArray(i32(this, 0x20));
    if (i32(this, 0x24) >= 0)
        glDisableVertexAttribArray(i32(this, 0x24));
    if (i32(this, 0x28) >= 0)
        glDisableVertexAttribArray(i32(this, 0x28));
    if (i32(this, 0x2c) >= 0)
        glDisableVertexAttribArray(i32(this, 0x2c));
    if (i32(this, 0x30) >= 0)
        glDisableVertexAttribArray(i32(this, 0x30));
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8e980.cpp ----
namespace AbyssEngine {

// AbyssEngine::GenericShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void GenericShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    if (i32(this, 0x34) >= 0)
        glUniformMatrix4fv(i32(this, 0x34), 1, 0, (const float *)(e + 0x104));
    if (i32(this, 0x38) >= 0)
        glUniformMatrix3fv(i32(this, 0x38), 1, 0, (const float *)(e + 0x204));

    if (u8(this, 0x9) != 0) {
        if (i32(this, 0x3c) >= 0)
            glUniform3f(i32(this, 0x3c), f32(e, 0x330), f32(e, 0x334), f32(e, 0x338));
        if (i32(this, 0x40) >= 0)
            glUniform3f(i32(this, 0x40), f32(e, 0x34c), f32(e, 0x350), f32(e, 0x354));
        if (i32(this, 0x50) >= 0)
            glUniform4fv(i32(this, 0x50), 1, (const float *)(e + 0xd0));
        if (i32(this, 0x54) >= 0)
            glUniform3fv(i32(this, 0x54), 1, (const float *)(e + 0x2cc));
        if (i32(this, 0x58) >= 0)
            glUniform3fv(i32(this, 0x58), 1, (const float *)(e + 0x2fc));
        if (i32(this, 0x5c) >= 0)
            glUniform3fv(i32(this, 0x5c), 1, (const float *)(e + 0x2e4));
        if (i32(this, 0x60) >= 0)
            glUniform1f(i32(this, 0x60), f32(e, 0x2c8));
        u8(this, 0x9) = 0;
    }

    if (i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(i32(this, 0x20));
    if (i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(i32(this, 0x24));
    if (i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(i32(this, 0x28));
    if (i32(this, 0x2c) >= 0)
        glEnableVertexAttribArray(i32(this, 0x2c));
    if (i32(this, 0x30) >= 0)
        glEnableVertexAttribArray(i32(this, 0x30));

    if (u8(mesh, 0x5c) != 0) {
        void (*bindBuffer)(unsigned int, unsigned int) = glBindBuffer;
        void (*vertexAttribPointer)(int, int, unsigned int, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, i32(m, 0x60));
        vertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(m, 0x68));
        vertexAttribPointer(i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(m, 0x6c));
        vertexAttribPointer(i32(this, 0x28), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(m, 0x70));
        vertexAttribPointer(i32(this, 0x2c), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(m, 0x74));
        vertexAttribPointer(i32(this, 0x30), 3, 0x1406, 0, 0, 0);
        return;
    }

    if (i32(this, 0x20) >= 0)
        glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, *(void **)(m + 0x4));
    if (i32(this, 0x24) >= 0)
        glVertexAttribPointer(i32(this, 0x24), 2, 0x1406, 0, 0, *(void **)(m + 0x8));
    if (i32(this, 0x28) >= 0)
        glVertexAttribPointer(i32(this, 0x28), 3, 0x1406, 0, 0, *(void **)(m + 0x10));
    if (i32(this, 0x2c) >= 0)
        glVertexAttribPointer(i32(this, 0x2c), 3, 0x1406, 0, 0, *(void **)(m + 0x14));
    if (i32(this, 0x30) >= 0)
        glVertexAttribPointer(i32(this, 0x30), 3, 0x1406, 0, 0, *(void **)(m + 0x18));
}

} // namespace AbyssEngine

// ---- GenericShader_8e750.cpp ----
namespace AbyssEngine {

// AbyssEngine::GenericShader::GenericShader()
GenericShader::GenericShader()
{
    // Base ShaderBaseStruct ctor.
    ShaderBaseStruct_ctor(this);

    // Install the GenericShader vtable (object pointer points 8 bytes into the
    // vtable table, past the RTTI/offset-to-top slots).
    *(void **)this = (void *)(GenericShader_vtable + 8);

    // Register this shader into the global registry list.
    *GenericShader_registerDst = GenericShader_registerSrc;

    // Initialise the String member at +0xc from a literal.
    String tmp("GenericShader", false);
    this->field_0xc = tmp;
    tmp.~String();
}

} // namespace AbyssEngine
