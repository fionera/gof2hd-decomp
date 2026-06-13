#include "gof2/GenericShader.h"
#include "gof2/ShaderBaseStruct.h"
#include "gof2/String.h"

// Registry slot the constructor links itself into (engine-global, resolved at link).
extern "C" void *GenericShader_registerSrc;
extern "C" void **GenericShader_registerDst;

namespace AbyssEngine {

// ---- Init_8e7dc.cpp ----
void GenericShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)(this))->ES2LoadProgram("GenericShader.vsh", "GenericShader.fsh");
    field_0x4 = program;

    field_0x20 = glGetAttribLocation(program, "a0");
    field_0x24 = glGetAttribLocation(field_0x4, "a1");
    field_0x28 = glGetAttribLocation(field_0x4, "a2");
    field_0x2c = glGetAttribLocation(field_0x4, "a3");
    field_0x30 = glGetAttribLocation(field_0x4, "a4");

    field_0x34 = glGetUniformLocation(field_0x4, "u0");
    field_0x38 = glGetUniformLocation(field_0x4, "u1");
    field_0x3c = glGetUniformLocation(field_0x4, "u2");
    field_0x40 = glGetUniformLocation(field_0x4, "u3");
    field_0x44 = glGetUniformLocation(field_0x4, "u4");
    field_0x48 = glGetUniformLocation(field_0x4, "u5");
    field_0x4c = glGetUniformLocation(field_0x4, "u6");
    field_0x50 = glGetUniformLocation(field_0x4, "u7");
    field_0x54 = glGetUniformLocation(field_0x4, "u8");
    field_0x58 = glGetUniformLocation(field_0x4, "u9");
    field_0x5c = glGetUniformLocation(field_0x4, "u10");
    field_0x60 = glGetUniformLocation(field_0x4, "u11");

    glUseProgram(field_0x4);
    // bind the three texture sampler uniforms u4,u5,u6 to texture units 0,1,2.
    int32_t *samplers = &field_0x44;
    for (int i = 0; i != 3; i++) {
        if (samplers[i] >= 0)
            glUniform1i(samplers[i], i);
    }
}

// ---- SetInActive_8e940.cpp ----
void GenericShader::SetInActive()
{
    if (field_0x20 >= 0)
        glDisableVertexAttribArray(field_0x20);
    if (field_0x24 >= 0)
        glDisableVertexAttribArray(field_0x24);
    if (field_0x28 >= 0)
        glDisableVertexAttribArray(field_0x28);
    if (field_0x2c >= 0)
        glDisableVertexAttribArray(field_0x2c);
    if (field_0x30 >= 0)
        glDisableVertexAttribArray(field_0x30);
}

// ---- UpdateMeshData_8e980.cpp ----
// AbyssEngine::GenericShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
// The Engine/Mesh objects carry the per-frame matrices, light data and GL vertex
// buffers at fixed offsets; those are foreign objects (other batches), read as raw
// GL data here.
void GenericShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    if (field_0x34 >= 0)
        glUniformMatrix4fv(field_0x34, 1, 0, (const float *)(e + 0x104));
    if (field_0x38 >= 0)
        glUniformMatrix3fv(field_0x38, 1, 0, (const float *)(e + 0x204));

    if (field_0x9 != 0) {
        if (field_0x3c >= 0)
            glUniform3f(field_0x3c, *(float *)(e + 0x330), *(float *)(e + 0x334), *(float *)(e + 0x338));
        if (field_0x40 >= 0)
            glUniform3f(field_0x40, *(float *)(e + 0x34c), *(float *)(e + 0x350), *(float *)(e + 0x354));
        if (field_0x50 >= 0)
            glUniform4fv(field_0x50, 1, (const float *)(e + 0xd0));
        if (field_0x54 >= 0)
            glUniform3fv(field_0x54, 1, (const float *)(e + 0x2cc));
        if (field_0x58 >= 0)
            glUniform3fv(field_0x58, 1, (const float *)(e + 0x2fc));
        if (field_0x5c >= 0)
            glUniform3fv(field_0x5c, 1, (const float *)(e + 0x2e4));
        if (field_0x60 >= 0)
            glUniform1f(field_0x60, *(float *)(e + 0x2c8));
        field_0x9 = 0;
    }

    if (field_0x20 >= 0)
        glEnableVertexAttribArray(field_0x20);
    if (field_0x24 >= 0)
        glEnableVertexAttribArray(field_0x24);
    if (field_0x28 >= 0)
        glEnableVertexAttribArray(field_0x28);
    if (field_0x2c >= 0)
        glEnableVertexAttribArray(field_0x2c);
    if (field_0x30 >= 0)
        glEnableVertexAttribArray(field_0x30);

    if (*(uint8_t *)(m + 0x5c) != 0) {
        void (*bindBuffer)(unsigned int, unsigned int) = glBindBuffer;
        void (*vertexAttribPointer)(int, int, unsigned int, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, *(int *)(m + 0x60));
        vertexAttribPointer(field_0x20, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, *(int *)(m + 0x68));
        vertexAttribPointer(field_0x24, 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, *(int *)(m + 0x6c));
        vertexAttribPointer(field_0x28, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, *(int *)(m + 0x70));
        vertexAttribPointer(field_0x2c, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, *(int *)(m + 0x74));
        vertexAttribPointer(field_0x30, 3, 0x1406, 0, 0, 0);
        return;
    }

    if (field_0x20 >= 0)
        glVertexAttribPointer(field_0x20, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
    if (field_0x24 >= 0)
        glVertexAttribPointer(field_0x24, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
    if (field_0x28 >= 0)
        glVertexAttribPointer(field_0x28, 3, 0x1406, 0, 0, *(void **)(m + 0x10));
    if (field_0x2c >= 0)
        glVertexAttribPointer(field_0x2c, 3, 0x1406, 0, 0, *(void **)(m + 0x14));
    if (field_0x30 >= 0)
        glVertexAttribPointer(field_0x30, 3, 0x1406, 0, 0, *(void **)(m + 0x18));
}

// ---- GenericShader_8e750.cpp ----
// AbyssEngine::GenericShader::GenericShader()
GenericShader::GenericShader()
{
    // Base ShaderBaseStruct ctor.
    new ((AbyssEngine::ShaderBaseStruct *)this) ShaderBaseStruct();

    // Install the GenericShader vtable (object pointer points 8 bytes into the
    // vtable table, past the RTTI/offset-to-top slots).
    field_0x0 = (void *)(GenericShader_vtable + 8);

    // Register this shader into the global registry list.
    *GenericShader_registerDst = GenericShader_registerSrc;

    // Initialise the String member at +0xc from a literal.
    String tmp;
    ((String *)(&tmp))->ctor_char("GenericShader", false);
    ((String *)(&field_0xc))->assign(&tmp);
    ((String *)(&tmp))->dtor();
}

} // namespace AbyssEngine

// ---- _GenericShader_8eb90.cpp ----
void _ZN11AbyssEngine13GenericShaderD0Ev(AbyssEngine::GenericShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    ::operator delete(base);
}
