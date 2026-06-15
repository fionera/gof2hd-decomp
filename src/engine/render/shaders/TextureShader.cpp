#include "gof2/engine/render/shaders/TextureShader.h"
#include "gof2/platform/gl.h"

// TextureShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" void *g_TextureShader_vtable;

// Engine globals: selects the extended program and the per-frame active texture slot.
extern "C" uint8_t g_TextureShader_hasSecondProgram;
extern "C" uint8_t g_TextureShader_activeSlot;

// Engine vector accessor reached by opaque pointer (modelled in a later pass).
namespace AbyssEngine {
namespace AEMath {
struct Vector;
}
}
extern "C" float *Vector_to_float(AbyssEngine::AEMath::Vector *self);

namespace AbyssEngine {

TextureShader::TextureShader()
{
    this->vtable = (char *)&g_TextureShader_vtable + 8;
    this->name.s = u"TextureShader";
}

// Compiles the base and extended GLES2 programs and caches each program's
// attribute/uniform locations into slot 0 and slot 1 respectively.
void TextureShader::Init(Engine *)
{
    const char *vertexShader = "TextureShader.vsh";

    this->program = this->ES2LoadProgram(vertexShader, "TextureShader.fsh");
    ConnectShaderComponents(this->program, 0);

    this->programExt = this->ES2LoadProgram(vertexShader, "TextureShaderExt.fsh");
    ConnectShaderComponents(this->programExt, 1);
}

void TextureShader::UseShader(bool)
{
    if (g_TextureShader_hasSecondProgram != 0 && this->programExt != 0) {
        glUseProgram(this->programExt);
        return;
    }
    glUseProgram(this->program);
}

void TextureShader::SetInActive()
{
    for (uint32_t i = 0; i != 2; i += 1) {
        glDisableVertexAttribArray(this->positionAttrib[i]);
        glDisableVertexAttribArray(this->texcoordAttrib[i]);
    }
}

void TextureShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    int slot = g_TextureShader_activeSlot;
    char *meshBytes = (char *)mesh;
    char *engineBytes = (char *)engine;

    glUniformMatrix4fv(this->mvpUniform[slot], 1, 0, (float *)(engineBytes + 0x104));

    int location = this->worldViewUniform[slot];
    if (location >= 0) {
        glUniformMatrix4fv(location, 1, 0, (float *)(engineBytes + 0x1c4));
    }

    location = this->alphaUniform[slot];
    if (location >= 0) {
        glUniform1f(location, 1.0f - *(float *)(meshBytes + 0x1c));
    }

    if (this->dirty != 0) {
        glUniform4fv(this->colorUniform[slot], 1, (float *)(engineBytes + 0xd0));

        location = this->textureModeUniform[slot];
        if (location >= 0) {
            glUniform1i(location, *(uint8_t *)(meshBytes + 0x85));
        }

        location = this->lightUniform[slot];
        if (location >= 0) {
            glUniform3fv(location, 1, Vector_to_float((AEMath::Vector *)(engineBytes + 0x3f0)));
        }

        location = this->fogFarUniform[slot];
        if (location >= 0) {
            glUniform1f(location, *(float *)(engineBytes + 0x3e8));
        }

        location = this->fogNearUniform[slot];
        if (location >= 0) {
            glUniform1f(location, *(float *)(engineBytes + 0x3ec));
        }

        location = this->activeTextureUniform[slot];
        if (location >= 0) {
            glUniform1i(location, g_TextureShader_activeSlot);
        }

        location = this->fogColorUniform[slot];
        if (location >= 0) {
            glUniform3f(location, *(float *)(engineBytes + 0x34c), *(float *)(engineBytes + 0x350),
                        *(float *)(engineBytes + 0x354));
        }

        this->dirty = 0;
    }

    if ((int)(*(uint8_t *)meshBytes << 30) < 0) {
        glEnableVertexAttribArray(this->positionAttrib[slot]);
        glEnableVertexAttribArray(this->texcoordAttrib[slot]);

        if (*(uint8_t *)(meshBytes + 0x5c) == 0) {
            glVertexAttribPointer(this->positionAttrib[slot], 3, 0x1406, 0, 0, *(void **)(meshBytes + 0x04));
            glVertexAttribPointer(this->texcoordAttrib[slot], 2, 0x1406, 0, 0, *(void **)(meshBytes + 0x08));
        } else {
            glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x60));
            glVertexAttribPointer(this->positionAttrib[slot], 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x68));
            glVertexAttribPointer(this->texcoordAttrib[slot], 2, 0x1406, 0, 0, 0);
        }
    }
}

void TextureShader::ConnectShaderComponents(uint32_t program, int slot)
{
    this->textureUniform[slot] = glGetUniformLocation(program, "texture");
    this->positionAttrib[slot] = glGetAttribLocation(program, "position");
    this->texcoordAttrib[slot] = glGetAttribLocation(program, "texcoord");
    this->mvpUniform[slot] = glGetUniformLocation(program, "mvp");
    this->colorUniform[slot] = glGetUniformLocation(program, "color");
    this->lightUniform[slot] = glGetUniformLocation(program, "light");
    this->fogNearUniform[slot] = glGetUniformLocation(program, "fogNear");
    this->fogFarUniform[slot] = glGetUniformLocation(program, "fogFar");
    this->activeTextureUniform[slot] = glGetUniformLocation(program, "activeTexture");
    this->fogColorUniform[slot] = glGetUniformLocation(program, "fogColor");
    this->alphaUniform[slot] = glGetUniformLocation(program, "alpha");
    this->textureModeUniform[slot] = glGetUniformLocation(program, "textureMode");
    this->worldViewUniform[slot] = glGetUniformLocation(program, "worldView");

    glUseProgram(program);
    glUniform1i(this->textureUniform[slot], 0);
}

} // namespace AbyssEngine
