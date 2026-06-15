#include "gof2/engine/render/shaders/TextureLightShader.h"
#include "gof2/engine/render/Mesh.h"
#include "gof2/platform/gl.h"

// TextureLightShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char g_TextureLightShader_vtable;

namespace AbyssEngine {

// Reads of the Engine object are deferred to a later Engine-modeling pass; until then its
// fields are reached by raw byte offset.
static inline const float *ef(Engine *engine, unsigned off)
{
    return (const float *)((char *)engine + off);
}
static inline int ei(Engine *engine, unsigned off)
{
    return *(int *)((char *)engine + off);
}
static inline float eff(Engine *engine, unsigned off)
{
    return *(float *)((char *)engine + off);
}

TextureLightShader::TextureLightShader()
{
    this->vtable = (char *)&g_TextureLightShader_vtable + 8;
    this->name.s = u"TextureLightShader";
}

// Compiles the lit-texture program and caches its attribute/uniform location handles.
void TextureLightShader::Init(Engine *)
{
    this->program = this->ES2LoadProgram("TextureLightShader.vsh", "TextureLightShader.fsh");

    this->uModelMatrix = glGetUniformLocation(this->program, "u0");
    this->uHasScaleAnimation = glGetUniformLocation(this->program, "u1");

    this->aPosition = glGetAttribLocation(this->program, "a0");
    this->aTexCoord = glGetAttribLocation(this->program, "a1");

    this->aNormal = glGetUniformLocation(this->program, "u2");
    this->uMvpMatrix = glGetUniformLocation(this->program, "u3");
    this->uModelViewMatrix = glGetUniformLocation(this->program, "u4");
    this->uNormalMatrix = glGetUniformLocation(this->program, "u5");
    this->uColor0 = glGetUniformLocation(this->program, "u6");
    this->uColor1 = glGetUniformLocation(this->program, "u7");
    this->uSpecularColor = glGetUniformLocation(this->program, "u8");
    this->uTexture = glGetUniformLocation(this->program, "u9");
    this->uAmbientColor = glGetUniformLocation(this->program, "u10");
    this->uLight0Direction = glGetUniformLocation(this->program, "u11");
    this->uLight1Direction = glGetUniformLocation(this->program, "u12");
    this->uLight2Direction = glGetUniformLocation(this->program, "u13");
    this->uShininess = glGetUniformLocation(this->program, "u14");
    this->uLight0Color = glGetUniformLocation(this->program, "u15");
    this->uLight1Color = glGetUniformLocation(this->program, "u16");
    this->uLight2Color = glGetUniformLocation(this->program, "u17");

    glUseProgram(this->program);
    glUniform1i(this->uTexture, 0);
}

// Binds the program's matrices, lighting block and the supplied mesh's vertex arrays.
void TextureLightShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(this->uMvpMatrix, 1, 0, ef(engine, 0x104));
    if (this->uModelMatrix >= 0)
        glUniformMatrix4fv(this->uModelMatrix, 1, 0, ef(engine, 0x1c4));
    glUniformMatrix3fv(this->uNormalMatrix, 1, 0, ef(engine, 0x204));
    if (this->uModelViewMatrix >= 0)
        glUniformMatrix4fv(this->uModelViewMatrix, 1, 0, ef(engine, 0x144));

    if (this->dirty != 0) {
        if (this->uHasScaleAnimation >= 0)
            glUniform1i(this->uHasScaleAnimation, mesh->hasAnimation);
        glUniform4fv(this->uAmbientColor, 1, ef(engine, 0xd0));

        glUniform3fv(this->uLight0Direction, 1, ef(engine, 0x2cc));
        glUniform3fv(this->uLight1Direction, 1, ef(engine, 0x2fc));
        glUniform3fv(this->uLight2Direction, 1, ef(engine, 0x2e4));

        glUniform4f(this->uColor0, eff(engine, 0x330), eff(engine, 0x334),
                    eff(engine, 0x338), eff(engine, 0x378));

        if (ei(engine, 0x32c) < 2) {
            glUniform3f(this->uLight0Color, 0, 0, 0);
            glUniform3f(this->uLight1Color, 0, 0, 0);
            glUniform3f(this->uLight2Color, 0, 0, 0);
        } else {
            glUniform3fv(this->uLight0Color, 1, ef(engine, 0x2d8));
            glUniform3fv(this->uLight1Color, 1, ef(engine, 0x308));
            glUniform3fv(this->uLight2Color, 1, ef(engine, 0x2f0));
        }

        glUniform4f(this->uColor1, eff(engine, 0x33c), eff(engine, 0x340),
                    eff(engine, 0x344), eff(engine, 0x37c));
        glUniform1f(this->uShininess, eff(engine, 0x2c8));
        if (this->uSpecularColor >= 0)
            glUniform3f(this->uSpecularColor, eff(engine, 0x34c), eff(engine, 0x350),
                        eff(engine, 0x354));
        this->dirty = 0;
    }

    glEnableVertexAttribArray(this->aPosition);
    glEnableVertexAttribArray(this->aTexCoord);
    glEnableVertexAttribArray(this->aNormal);

    if (mesh->uploaded != 0) {
        glBindBuffer(0x8892, mesh->positionVBO);
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->texCoordVBO);
        glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->normalVBO);
        glVertexAttribPointer(this->aNormal, 3, 0x1406, 0, 0, 0);
        return;
    }

    glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, mesh->positions);
    glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, mesh->texCoords);
    glVertexAttribPointer(this->aNormal, 3, 0x1406, 0, 0, mesh->normals);
}

void TextureLightShader::SetInActive()
{
    glDisableVertexAttribArray(this->aPosition);
    glDisableVertexAttribArray(this->aTexCoord);
    glDisableVertexAttribArray(this->aNormal);
}

} // namespace AbyssEngine
