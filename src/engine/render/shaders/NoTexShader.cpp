#include "gof2/engine/render/shaders/NoTexShader.h"
#include "gof2/engine/render/Engine.h"
#include "gof2/engine/render/Mesh.h"
#include "gof2/platform/gl.h"

// NoTexShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char NoTexShader_vtable;

// Engine global: this shader's per-shader index, snapshotted at construction.
extern "C" int NoTexShader_ShaderIndex;

namespace AbyssEngine {

void NoTexShader::SetInActive()
{
    glDisableVertexAttribArray(this->aPosition);
}

void NoTexShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(this->uMvpMatrix, 1, 0, engine->worldViewProjMatrix);
    if (this->dirty != 0) {
        glUniform4fv(this->uColor, 1, engine->glColor);
        this->dirty = 0;
    }
    glEnableVertexAttribArray(this->aPosition);

    int size;
    const void *ptr;
    if (mesh == 0) {
        ptr = *(void **)&engine->field_0x348;
        size = 2;
    } else {
        if (mesh->uploaded == 0) {
            ptr = mesh->positions;
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            ptr = 0;
        }
        size = 3;
    }
    glVertexAttribPointer(this->aPosition, size, 0x1406, 0, 0, ptr);
}

void NoTexShader::Init(Engine *)
{
    this->program = this->ES2LoadProgram("NoTexShader.vsh", "NoTexShader.fsh");
    this->aPosition = glGetAttribLocation(this->program, "a_position");
    this->uMvpMatrix = glGetUniformLocation(this->program, "u_mvp");
    this->uColor = glGetUniformLocation(this->program, "u_color");
    glUseProgram(this->program);
}

NoTexShader::NoTexShader()
{
    this->vtable = &NoTexShader_vtable + 8;
    NoTexShader_ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"NoTexShader";
}

} // namespace AbyssEngine
