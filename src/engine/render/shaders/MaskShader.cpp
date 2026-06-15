#include "gof2/engine/render/shaders/MaskShader.h"
// Engine.h reaches these by unqualified (global-scope) name; satisfy it without modeling them.
class FBOContainer;
class ShaderBaseStruct;
#include "gof2/engine/render/Engine.h"
#include "gof2/engine/render/Mesh.h"
#include "gof2/platform/gl.h"

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
    if (this->u_mvpMatrix >= 0)
        glUniformMatrix4fv(this->u_mvpMatrix, 1, 0, engine->worldViewProjMatrix);

    if (this->dirty != 0) {
        if (this->u_color >= 0)
            glUniform4fv(this->u_color, 1, engine->glColor);
        this->dirty = 0;
    }

    if (this->a_position >= 0)
        glEnableVertexAttribArray(this->a_position);
    if (this->a_texCoord >= 0)
        glEnableVertexAttribArray(this->a_texCoord);
    if (this->a_color >= 0)
        glEnableVertexAttribArray(this->a_color);

    char *texBase = (char *)engine->field_0x30;
    int *tex = *(int **)(texBase + 0x20);
    if (tex != 0)
        ((::Engine *)engine)->SetTextureSlot(tex[1], 1);

    if (mesh->uploaded == 0) {
        if (this->a_position >= 0)
            glVertexAttribPointer(this->a_position, 3, 0x1406, 0, 0, mesh->positions);
        if (this->a_texCoord >= 0)
            glVertexAttribPointer(this->a_texCoord, 2, 0x1406, 0, 0, mesh->texCoords);
        if (tex == 0)
            return;
        if (this->a_color < 0)
            return;
        glVertexAttribPointer(this->a_color, 2, 0x1406, 0, 0, *(void **)(*tex + 8));
    } else {
        glBindBuffer(0x8892, mesh->positionVBO);
        glVertexAttribPointer(this->a_position, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->texCoordVBO);
        glVertexAttribPointer(this->a_texCoord, 2, 0x1406, 0, 0, 0);
        if (tex == 0)
            return;
        glBindBuffer(0x8892, *(uint32_t *)(*tex + 0x68));
        glVertexAttribPointer(this->a_color, 2, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
