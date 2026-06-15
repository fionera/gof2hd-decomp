#include "gof2/engine/render/shaders/TexOnlyShader.h"
#include "gof2/platform/gl.h"

// TexOnlyShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" void *_ZTVN11AbyssEngine12TexOnlyShaderE[];

namespace AbyssEngine {

TexOnlyShader::TexOnlyShader()
{
    this->vtable = (char *)_ZTVN11AbyssEngine12TexOnlyShaderE + 8;
    this->name.s = u"TexOnlyShader";
}

void TexOnlyShader::Init(Engine *)
{
    this->program = (int)this->ES2LoadProgram("TexOnlyShader.vsh", "TexOnlyShader.fsh");
    this->aPosition = glGetAttribLocation(this->program, "a_position");
    this->aTexCoord = glGetAttribLocation(this->program, "a_texCoord");
    this->uWorldMatrix = glGetUniformLocation(this->program, "u_WorldMatrix");
    this->sTexture = glGetUniformLocation(this->program, "s_texture");
    glUseProgram(this->program);
    glUniform1i(this->sTexture, 0);
}

void TexOnlyShader::SetInActive()
{
    glDisableVertexAttribArray(this->aPosition);
    glDisableVertexAttribArray(this->aTexCoord);
}

void TexOnlyShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    glUniformMatrix4fv(this->uWorldMatrix, 1, 0, (const float *)(e + 0x104));
    if (this->dirty != 0) {
        this->dirty = 0;
    }
    glEnableVertexAttribArray(this->aPosition);
    glEnableVertexAttribArray(this->aTexCoord);

    if (*(uint8_t *)(m + 0x5c) == 0) {
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
    } else {
        glBindBuffer(0x8892, *(uint32_t *)(m + 0x60));
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(uint32_t *)(m + 0x68));
        glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
