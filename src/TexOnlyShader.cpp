#include "gof2/TexOnlyShader.h"

// Engine / Mesh are opaque here; their fields are accessed by raw offset since
// their real layouts live in other (non-batch) classes.
static inline char *bytes(void *self) { return (char *)self; }

// ---- UpdateMeshData_8b74a.cpp ----
namespace AbyssEngine {

void TexOnlyShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(this->field_0x28, 1, 0, bytes(engine) + 0x104);
    if (this->field_0x9 != 0) {
        this->field_0x9 = 0;
    }
    glEnableVertexAttribArray(this->field_0x20);
    glEnableVertexAttribArray(this->field_0x24);

    char *meshBytes = bytes(mesh);
    if (*(uint8_t *)(meshBytes + 0x5c) == 0) {
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, *(void **)(meshBytes + 0x4));
        return glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, *(void **)(meshBytes + 0x8));
    } else {
        glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x60));
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x68));
        return glVertexAttribPointer(this->field_0x24, 2, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- SetInActive_8b734.cpp ----
namespace AbyssEngine {

void TexOnlyShader::SetInActive()
{
    glDisableVertexAttribArray(this->field_0x20);
    return glDisableVertexAttribArray(this->field_0x24);
}

} // namespace AbyssEngine

// ---- Init_8b6c0.cpp ----
namespace AbyssEngine {

void TexOnlyShader::Init(Engine *)
{
    uint32_t program = ShaderBaseStruct_ES2LoadProgram(this, "TexOnlyShader.vsh", "TexOnlyShader.fsh");
    this->field_0x4 = program;

    this->field_0x20 = glGetAttribLocation(program, "a_position");
    this->field_0x24 = glGetAttribLocation(this->field_0x4, "a_texCoord");
    this->field_0x28 = glGetUniformLocation(this->field_0x4, "u_WorldMatrix");
    this->field_0x2c = glGetUniformLocation(this->field_0x4, "s_texture");

    glUseProgram(this->field_0x4);
    return glUniform1i(this->field_0x2c, 0);
}

} // namespace AbyssEngine

// ---- TexOnlyShader_8b634.cpp ----
namespace AbyssEngine {

TexOnlyShader::TexOnlyShader()
{
    void **guard = (void **)&__stack_chk_guard;
    void *volatile cookie = *guard;
    ShaderBaseStruct_ctor(this);
    this->field_0x0 = (void *)((char *)g_TexOnlyShader_vtable + 8);
    g_TexOnlyShader_shaderIndex = g_ShaderBaseStruct_shaderIndexIntern;
    String tmp;
    String_ctor_char(&tmp, "TexOnlyShader", false);
    String_assign(&this->field_0xc, &tmp);
    String_dtor(&tmp);

    uint32_t guardDelta = (uint32_t)((__UINTPTR_TYPE__)*guard - (__UINTPTR_TYPE__)cookie);
    if (guardDelta != 0) {
        __stack_chk_fail(guardDelta);
    }
}

} // namespace AbyssEngine

// ---- _TexOnlyShader_8b7e2.cpp ----
extern "C" void _ZN11AbyssEngine12TexOnlyShaderD0Ev(AbyssEngine::TexOnlyShader *self)
{
    operator delete(ShaderBaseStruct_dtor(self));
}
