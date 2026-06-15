#include "gof2/engine/render/shaders/TextureAlphaTestShader.h"
#include "gof2/platform/gl.h"

// TextureAlphaTestShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char TextureAlphaTestShader_vtable;

// Engine globals: whether the alpha-test variant is active, and which program index (0 = opaque,
// 1 = alpha) is currently selected.
extern "C" unsigned char g_TextureAlphaTestShader_useAlphaProgram;
extern "C" unsigned char g_TextureAlphaTestShader_programIndex;

// AbyssEngine::Vector reinterpreted as a 3-float array (modelled in a later math pass).
extern "C" float *Vector_cast_to_float(AbyssEngine::AEMath::Vector *self);

namespace AbyssEngine {

// Cross-object reads of Engine/Mesh are deferred to a later Engine/Mesh-modeling pass; until
// then their fields are reached by raw offset.
template <class T> static inline T &ae_field(void *base, unsigned int off) {
    return *(T *)((char *)base + off);
}

int TextureAlphaTestShader::ShaderIndex;

TextureAlphaTestShader::TextureAlphaTestShader()
{
    this->vtable = &TextureAlphaTestShader_vtable + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"TextureAlphaTestShader";
}

// Compiles the opaque and alpha-test GLES2 programs and caches each program's locations.
void TextureAlphaTestShader::Init(Engine *)
{
    this->program = this->ES2LoadProgram("TextureAlphaTestShader.vsh", "TextureAlphaTestShader.fsh");
    ConnectShaderComponents(this->program, 0);

    this->alphaProgram = this->ES2LoadProgram("TextureAlphaTestShader.vsh", "TextureAlphaTestShaderAlpha.fsh");
    ConnectShaderComponents(this->alphaProgram, 1);
}

void TextureAlphaTestShader::SetInActive()
{
    for (int i = 0; i != 2; i++) {
        glDisableVertexAttribArray(this->aPositionLoc[i]);
        glDisableVertexAttribArray(this->aTexCoordLoc[i]);
    }
}

void TextureAlphaTestShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    uint8_t programIndex = g_TextureAlphaTestShader_programIndex;

    glUniformMatrix4fv(this->uMVPMatrixLoc[programIndex], 1, 0, (float *)((char *)engine + 0x104));

    if (this->dirty != 0) {
        int location = this->uColorLoc[programIndex];
        if (location >= 0) {
            glUniform4fv(location, 1, (float *)((char *)engine + 0xd0));
        }

        location = this->uLightPosLoc[programIndex];
        if (location >= 0) {
            glUniform3fv(location, 1, Vector_cast_to_float((AEMath::Vector *)((char *)engine + 0x3f0)));
        }

        location = this->uDiffuseLoc[programIndex];
        if (location >= 0) {
            glUniform1f(location, ae_field<float>(engine, 0x3e8));
        }

        location = this->uAmbientLoc[programIndex];
        if (location >= 0) {
            glUniform1f(location, ae_field<float>(engine, 0x3ec));
        }

        location = this->uSamplerLoc[programIndex];
        if (location >= 0) {
            glUniform1i(location, g_TextureAlphaTestShader_programIndex);
        }

        location = this->uFogColorLoc[programIndex];
        if (location >= 0) {
            glUniform3f(location, ae_field<float>(engine, 0x34c), ae_field<float>(engine, 0x350),
                        ae_field<float>(engine, 0x354));
        }

        this->dirty = 0;
    }

    if ((((uint32_t)ae_field<uint8_t>(mesh, 0x00)) << 30) < 0x80000000u) {
        return;
    }

    int position = this->aPositionLoc[programIndex];
    int texcoord = this->aTexCoordLoc[programIndex];
    glEnableVertexAttribArray(position);
    glEnableVertexAttribArray(texcoord);

    if (ae_field<uint8_t>(mesh, 0x5c) == 0) {
        glVertexAttribPointer(position, 3, 0x1406, 0, 0, ae_field<void *>(mesh, 0x4));
        glVertexAttribPointer(texcoord, 2, 0x1406, 0, 0, ae_field<void *>(mesh, 0x8));
    } else {
        glBindBuffer(0x8892, ae_field<unsigned int>(mesh, 0x60));
        glVertexAttribPointer(position, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, ae_field<unsigned int>(mesh, 0x68));
        glVertexAttribPointer(texcoord, 2, 0x1406, 0, 0, 0);
    }
}

void TextureAlphaTestShader::ConnectShaderComponents(int program, int index)
{
    this->uTextureLoc[index] = glGetUniformLocation(program, "u_Texture");
    this->aPositionLoc[index] = glGetAttribLocation(program, "a_Position");
    this->aTexCoordLoc[index] = glGetAttribLocation(program, "a_TexCoord");
    this->uMVPMatrixLoc[index] = glGetUniformLocation(program, "u_MVPMatrix");
    this->uColorLoc[index] = glGetUniformLocation(program, "u_Color");
    this->uLightPosLoc[index] = glGetUniformLocation(program, "u_LightPos");
    this->uAmbientLoc[index] = glGetUniformLocation(program, "u_Ambient");
    this->uDiffuseLoc[index] = glGetUniformLocation(program, "u_Diffuse");
    this->uSamplerLoc[index] = glGetUniformLocation(program, "u_Sampler");
    this->uFogColorLoc[index] = glGetUniformLocation(program, "u_FogColor");

    glUseProgram(program);
    glUniform1i(this->uTextureLoc[index], 0);
}

void TextureAlphaTestShader::UseShader(bool)
{
    if (g_TextureAlphaTestShader_useAlphaProgram != 0) {
        int program = this->alphaProgram;
        if (program != 0) {
            glUseProgram(program);
            return;
        }
    }
    glUseProgram(this->program);
}

} // namespace AbyssEngine
