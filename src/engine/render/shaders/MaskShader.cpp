#include "gof2/engine/render/shaders/MaskShader.h"
// Engine.h reaches these by unqualified (global-scope) name; satisfy it without modeling them.
class FBOContainer;
class ShaderBaseStruct;
#include "gof2/engine/render/Engine.h"
#include "gof2/platform/gl.h"

// Cross-object reads of Engine/Mesh are deferred to a later Engine/Mesh-modeling pass; until
// then their fields are reached by raw offset.

namespace AbyssEngine {

// MaskShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char MaskShader_vtable;

int MaskShader::ShaderIndex;

MaskShader::MaskShader()
{
    this->vtable = &MaskShader_vtable + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"MaskShader";
}

void MaskShader::Init(::Engine *)
{
    this->program = (int)ES2LoadProgram("MaskShader.vsh", "MaskShader.fsh");

    this->a_position = glGetAttribLocation(this->program, "a_position");
    this->a_texCoord = glGetAttribLocation(this->program, "a_texCoord");
    this->a_color = glGetAttribLocation(this->program, "a_color");

    this->u_mvpMatrix = glGetUniformLocation(this->program, "u_mvpMatrix");
    this->u_texture0 = glGetUniformLocation(this->program, "u_texture0");
    this->u_texture1 = glGetUniformLocation(this->program, "u_texture1");
    this->u_color = glGetUniformLocation(this->program, "u_color");

    glUseProgram(this->program);
    for (int i = 0; i != 2; i++) {
        int loc = (&this->u_texture0)[i];
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

void MaskShader::SetInActive()
{
    if (this->a_position >= 0)
        glDisableVertexAttribArray(this->a_position);
    if (this->a_texCoord >= 0)
        glDisableVertexAttribArray(this->a_texCoord);
    if (this->a_color >= 0)
        glDisableVertexAttribArray(this->a_color);
}

void MaskShader::UpdateMeshData(Mesh *mesh, ::Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    if (this->u_mvpMatrix >= 0)
        glUniformMatrix4fv(this->u_mvpMatrix, 1, 0, (const float *)(e + 0x104));

    if (this->dirty != 0) {
        if (this->u_color >= 0)
            glUniform4fv(this->u_color, 1, (const float *)(e + 0xd0));
        this->dirty = 0;
    }

    if (this->a_position >= 0)
        glEnableVertexAttribArray(this->a_position);
    if (this->a_texCoord >= 0)
        glEnableVertexAttribArray(this->a_texCoord);
    if (this->a_color >= 0)
        glEnableVertexAttribArray(this->a_color);

    char *texBase = *(char **)(e + 0x30);
    int *tex = *(int **)(texBase + 0x20);
    if (tex != 0)
        ((::Engine *)engine)->SetTextureSlot(tex[1], 1);

    if (*(uint8_t *)(m + 0x5c) == 0) {
        if (this->a_position >= 0)
            glVertexAttribPointer(this->a_position, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (this->a_texCoord >= 0)
            glVertexAttribPointer(this->a_texCoord, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        if (tex == 0)
            return;
        if (this->a_color < 0)
            return;
        glVertexAttribPointer(this->a_color, 2, 0x1406, 0, 0, *(void **)(*tex + 8));
    } else {
        glBindBuffer(0x8892, *(uint32_t *)(m + 0x60));
        glVertexAttribPointer(this->a_position, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(uint32_t *)(m + 0x68));
        glVertexAttribPointer(this->a_texCoord, 2, 0x1406, 0, 0, 0);
        if (tex == 0)
            return;
        glBindBuffer(0x8892, *(uint32_t *)(*tex + 0x68));
        glVertexAttribPointer(this->a_color, 2, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
