#include "gof2/engine/render/shaders/NoTexVtxColorShader.h"
#include "gof2/engine/render/Engine.h"
#include "gof2/engine/render/Mesh.h"
#include "gof2/platform/gl.h"

// NoTexVtxColorShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char NoTexVtxColorShader_vtable;

// Per-shader index snapshot taken at construction time.
extern "C" int32_t NoTexVtxColorShader_ShaderIndex;

namespace AbyssEngine {

void NoTexVtxColorShader::SetInActive()
{
    glDisableVertexAttribArray(aPosition);
    glDisableVertexAttribArray(aColor);
}

void NoTexVtxColorShader::Init(Engine *)
{
    this->program = this->ES2LoadProgram("NoTexVtxColorShader.vsh", "NoTexVtxColorShader.fsh");

    aPosition = glGetAttribLocation(this->program, "a_position");
    aColor = glGetAttribLocation(this->program, "a_color");

    uMvpMatrix = glGetUniformLocation(this->program, "u_mvp");
    uColor = glGetUniformLocation(this->program, "u_color");

    glUseProgram(this->program);
}

NoTexVtxColorShader::NoTexVtxColorShader()
{
    this->vtable = &NoTexVtxColorShader_vtable + 8;
    NoTexVtxColorShader_ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"NoTexVtxColorShader";
}

void NoTexVtxColorShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(uMvpMatrix, 1, 0, engine->worldViewProjMatrix);
    if (this->dirty != 0) {
        glUniform4fv(uColor, 1, engine->glColor);
        this->dirty = 0;
    }
    glEnableVertexAttribArray(aPosition);
    glEnableVertexAttribArray(aColor);

    int index = aPosition;
    int size;
    const void *ptr;
    int stride = 0;
    if (mesh == 0) {
        ptr = *(void **)&engine->field_0x348;
        size = 2;
    } else {
        glVertexAttribPointer(index, 3, 0x1406, 0, stride, mesh->positions);
        ptr = mesh->colors;
        index = aColor;
        size = 4;
    }
    glVertexAttribPointer(index, size, 0x1406, 0, stride, ptr);
}

} // namespace AbyssEngine
