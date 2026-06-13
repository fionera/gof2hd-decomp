#include "gof2/engine/render/shaders/TexOnlyShader.h"
#include "gof2/engine/render/ShaderBaseStruct.h"
#include "gof2/game/core/String.h"

// Engine / Mesh are opaque here; their fields are accessed by raw offset since
// their real layouts live in other (non-batch) classes.
static inline char *bytes(void *self) { return (char *)self; }

namespace AbyssEngine {

void TexOnlyShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(this->uWorldMatrix, 1, 0, bytes(engine) + 0x104);
    if (this->uniformsDirty != 0) {
        this->uniformsDirty = 0;
    }
    glEnableVertexAttribArray(this->aPosition);
    glEnableVertexAttribArray(this->aTexCoord);

    char *meshBytes = bytes(mesh);
    if (*(uint8_t *)(meshBytes + 0x5c) == 0) {
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, *(void **)(meshBytes + 0x4));
        return glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, *(void **)(meshBytes + 0x8));
    } else {
        glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x60));
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x68));
        return glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

namespace AbyssEngine {

void TexOnlyShader::SetInActive()
{
    glDisableVertexAttribArray(this->aPosition);
    return glDisableVertexAttribArray(this->aTexCoord);
}

} // namespace AbyssEngine

namespace AbyssEngine {

void TexOnlyShader::Init(Engine *)
{
    uint32_t program = ((ShaderBaseStruct *)(this))->ES2LoadProgram("TexOnlyShader.vsh", "TexOnlyShader.fsh");
    this->program = program;

    this->aPosition = glGetAttribLocation(program, "a_position");
    this->aTexCoord = glGetAttribLocation(this->program, "a_texCoord");
    this->uWorldMatrix = glGetUniformLocation(this->program, "u_WorldMatrix");
    this->sTexture = glGetUniformLocation(this->program, "s_texture");

    glUseProgram(this->program);
    return glUniform1i(this->sTexture, 0);
}

} // namespace AbyssEngine

namespace AbyssEngine {

TexOnlyShader::TexOnlyShader()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    this->field_0x0 = (void *)((char *)g_TexOnlyShader_vtable + 8);
    g_TexOnlyShader_shaderIndex = ShaderBaseStruct::shaderIndexIntern;
    String tmp;
    ((String *)(&tmp))->ctor_char("TexOnlyShader", false);
    ((String *)(&this->name))->assign(&tmp);
    ((String *)(&tmp))->dtor();

    
}

} // namespace AbyssEngine

void _ZN11AbyssEngine12TexOnlyShaderD0Ev(AbyssEngine::TexOnlyShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}
