#include "SandboxShader.h"




// ---- _SandboxShader_91ad0.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine13SandboxShaderD0Ev(
    AbyssEngine::SandboxShader *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- UpdateMeshData_9194c.cpp ----
namespace AbyssEngine {

void SandboxShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_i32(this, 0x34), 1, 0, (char *)engine + 0x104);

    if (field_u8(this, 0x9) != 0) {
        glUniform4fv(field_i32(this, 0x40), 1, (float *)((char *)engine + 0xd0));
        glUniform3f(field_i32(this, 0x38), field_f32(engine, 0x330), field_f32(engine, 0x334),
                    field_f32(engine, 0x338));
        glUniform3f(field_i32(this, 0x3c), field_f32(engine, 0x34c), field_f32(engine, 0x350),
                    field_f32(engine, 0x354));
        field_u8(this, 0x9) = 0;
    }

    void (*enableVertexAttribArray)(uint32_t) = glEnableVertexAttribArray;
    enableVertexAttribArray(field_i32(this, 0x20));
    enableVertexAttribArray(field_i32(this, 0x24));
    enableVertexAttribArray(field_i32(this, 0x28));
    enableVertexAttribArray(field_i32(this, 0x2c));
    enableVertexAttribArray(field_i32(this, 0x30));

    if (field_u8(mesh, 0x5c) != 0) {
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
        return;
    }

    void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
        glVertexAttribPointer;
    vertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    vertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
    vertexAttribPointer(field_i32(this, 0x28), 3, 0x1406, 0, 0, field_ptr(mesh, 0x10));
    vertexAttribPointer(field_i32(this, 0x2c), 3, 0x1406, 0, 0, field_ptr(mesh, 0x14));
    vertexAttribPointer(field_i32(this, 0x30), 3, 0x1406, 0, 0, field_ptr(mesh, 0x18));
}

} // namespace AbyssEngine

// ---- Init_91818.cpp ----
namespace AbyssEngine {

void SandboxShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("SandboxShader.vsh", "SandboxShader.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_normal");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a_tangent");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a_binormal");
    field_i32(this, 0x30) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");

    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u_a");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u_b");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u_c");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u_d");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u_e");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u_f");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u_g");

    glUseProgram(field_i32(this, 0x04));
    glUniform1i(field_i32(this, 0x44), 0);
    return glUniform1i(field_i32(this, 0x48), 1);
}

} // namespace AbyssEngine

// ---- SetInActive_91920.cpp ----
namespace AbyssEngine {

void SandboxShader::SetInActive()
{
    void (*disableVertexAttribArray)(uint32_t) = glDisableVertexAttribArray;

    disableVertexAttribArray(field_i32(this, 0x20));
    disableVertexAttribArray(field_i32(this, 0x24));
    disableVertexAttribArray(field_i32(this, 0x28));
    disableVertexAttribArray(field_i32(this, 0x2c));
    disableVertexAttribArray(field_i32(this, 0x30));
}

} // namespace AbyssEngine

// ---- SandboxShader_9178c.cpp ----
namespace AbyssEngine {

// AbyssEngine::SandboxShader::SandboxShader()
SandboxShader::SandboxShader()
{
    // Base ShaderBaseStruct ctor.
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);

    // Install the SandboxShader vtable (object pointer points 8 bytes into the
    // vtable, past the RTTI / offset-to-top slots).
    *(void **)this = (void *)(_ZTVN11AbyssEngine13SandboxShaderE + 8);

    // Register this shader into the global registry list.
    *SandboxShader_registerDst = SandboxShader_registerSrc;

    // Initialise the String member at +0xc from a literal.
    String tmp("SandboxShader", false);
    *(String *)((char *)this + 0xc) = tmp;
    tmp.~String();
}

} // namespace AbyssEngine
