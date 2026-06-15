#include "gof2/engine/render/shaders/SimpleShader.h"
#include "gof2/engine/render/Engine.h"
#include "gof2/engine/render/Mesh.h"
#include "gof2/platform/gl.h"

// SimpleShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" void *_ZTVN11AbyssEngine12SimpleShaderE[];

namespace AbyssEngine {

SimpleShader::SimpleShader()
{
    this->vtable = (char *)_ZTVN11AbyssEngine12SimpleShaderE + 8;
    this->name.s = u"SimpleShader";
}

void SimpleShader::Init(Engine *)
{
    this->program = (int)this->ES2LoadProgram("SimpleShader.vsh", "SimpleShader.fsh");
    this->aPosition = glGetAttribLocation(this->program, "a_position");
    this->uWorldMatrix = glGetUniformLocation(this->program, "u_WorldMatrix");
    this->uColor = glGetUniformLocation(this->program, "u_color");
    glUseProgram(this->program);
}

void SimpleShader::SetInActive()
{
    glDisableVertexAttribArray(this->aPosition);
}

void SimpleShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(this->uWorldMatrix, 1, 0, engine->worldViewProjMatrix);
    if (this->dirty != 0) {
        glUniform4fv(this->uColor, 1, engine->glColor);
        this->dirty = 0;
    }
    glEnableVertexAttribArray(this->aPosition);
    if (mesh->uploaded != 0) {
        glBindBuffer(0x8892, mesh->positionVBO);
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
        return;
    }
    glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, mesh->positions);
}

} // namespace AbyssEngine
