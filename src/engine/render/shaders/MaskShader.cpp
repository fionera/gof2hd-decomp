#include "gof2/engine/render/shaders/MaskShader.h"
#include "gof2/engine/render/Engine.h"

namespace AbyssEngine {

int MaskShader::ShaderIndex;

// AbyssEngine::MaskShader::SetInActive()
void MaskShader::SetInActive()
{
    if (field_0x20 >= 0)
        glDisableVertexAttribArray(field_0x20);
    if (field_0x24 >= 0)
        glDisableVertexAttribArray(field_0x24);
    if (field_0x28 >= 0)
        glDisableVertexAttribArray(field_0x28);
}

// AbyssEngine::MaskShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void MaskShader::UpdateMeshData(Mesh *mesh, ::Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    if (field_0x2c >= 0)
        glUniformMatrix4fv(field_0x2c, 1, 0, e + 0x104);

    if (field_0x9 != 0) {
        if (field_0x38 >= 0)
            glUniform4fv(field_0x38, 1, (const float *)(e + 0xd0));
        field_0x9 = 0;
    }

    if (field_0x20 >= 0)
        glEnableVertexAttribArray(field_0x20);
    if (field_0x24 >= 0)
        glEnableVertexAttribArray(field_0x24);
    if (field_0x28 >= 0)
        glEnableVertexAttribArray(field_0x28);

    char *texBase = *(char **)(e + 0x30);
    int *tex = *(int **)(texBase + 0x20);
    if (tex != 0)
        ((::Engine *)(engine))->SetTextureSlot(tex[1], 1);

    if (*(uint8_t *)(m + 0x5c) == 0) {
        if (field_0x20 >= 0)
            glVertexAttribPointer(field_0x20, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (field_0x24 >= 0)
            glVertexAttribPointer(field_0x24, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        if (tex == 0)
            return;
        if (field_0x28 < 0)
            return;
        glVertexAttribPointer(field_0x28, 2, 0x1406, 0, 0, *(void **)(*tex + 8));
    } else {
        glBindBuffer(0x8892, *(uint32_t *)(m + 0x60));
        glVertexAttribPointer(field_0x20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(uint32_t *)(m + 0x68));
        glVertexAttribPointer(field_0x24, 2, 0x1406, 0, 0, 0);
        if (tex == 0)
            return;
        glBindBuffer(0x8892, *(uint32_t *)(*tex + 0x68));
        glVertexAttribPointer(field_0x28, 2, 0x1406, 0, 0, 0);
    }
}

// AbyssEngine::MaskShader::Init(AbyssEngine::Engine*)
void MaskShader::Init(::Engine *)
{
    uint32_t program = ES2LoadProgram("MaskShader.vsh", "MaskShader.fsh");
    field_0x4 = (int)program;

    field_0x20 = glGetAttribLocation(program, "a_position");
    field_0x24 = glGetAttribLocation(field_0x4, "a_texCoord");
    field_0x28 = glGetAttribLocation(field_0x4, "a_color");

    field_0x2c = glGetUniformLocation(field_0x4, "u_mvpMatrix");
    field_0x30 = glGetUniformLocation(field_0x4, "u_texture0");
    field_0x34 = glGetUniformLocation(field_0x4, "u_texture1");
    field_0x38 = glGetUniformLocation(field_0x4, "u_color");

    glUseProgram(field_0x4);
    for (int i = 0; i != 2; i++) {
        int loc = (&field_0x30)[i];
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

// AbyssEngine::MaskShader::MaskShader()
MaskShader::MaskShader()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    field_0x0 = (char *)MaskShader_vtable + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    field_0xc.s = u"MaskShader";
    
}

} // namespace AbyssEngine

// ---- _MaskShader_9177c.cpp ----
// AbyssEngine::MaskShader::~MaskShader() (deleting dtor)
void _ZN11AbyssEngine10MaskShaderD0Ev(AbyssEngine::MaskShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    ::operator delete(base);
}
