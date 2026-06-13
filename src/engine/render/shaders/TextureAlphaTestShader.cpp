#include "gof2/engine/render/shaders/TextureAlphaTestShader.h"

// AbyssEngine::Mesh is forward-declared only (its full header conflicts with this TU's
// global typedefs), so read the few Mesh fields used here by typed offset.
static inline void *&meshPtr(void *m, int off) { return *(void **)((char *)m + off); }
static inline unsigned int &meshU32(void *m, int off) { return *(unsigned int *)((char *)m + off); }

void TextureAlphaTestShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("TextureAlphaTestShader.vsh", "TextureAlphaTestShader.fsh");
    this->field_0x4 = program;
    ConnectShaderComponents(program, 0);

    program = ((ShaderBaseStruct *)this)->ES2LoadProgram("TextureAlphaTestShader.vsh", "TextureAlphaTestShaderAlpha.fsh");
    this->alphaProgram = program;
    ConnectShaderComponents(program, 1);
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
            glUniform3fv(location, 1, Vector_cast_to_float((Vector *)((char *)engine + 0x3f0)));
        }

        location = this->uDiffuseLoc[programIndex];
        if (location >= 0) {
            glUniform1f(location, f32(engine, 0x3e8));
        }

        location = this->uAmbientLoc[programIndex];
        if (location >= 0) {
            glUniform1f(location, f32(engine, 0x3ec));
        }

        location = this->uSamplerLoc[programIndex];
        if (location >= 0) {
            glUniform1i(location, g_TextureAlphaTestShader_programIndex);
        }

        location = this->uFogColorLoc[programIndex];
        if (location >= 0) {
            glUniform3f(location, f32(engine, 0x34c), f32(engine, 0x350), f32(engine, 0x354));
        }

        this->dirty = 0;
    }

    if ((((uint32_t)u8(mesh, 0x00)) << 30) < 0x80000000u) {
        return;
    }

    int position = this->aPositionLoc[programIndex];
    int texcoord = this->aTexCoordLoc[programIndex];
    glEnableVertexAttribArray(position);
    glEnableVertexAttribArray(texcoord);

    if (u8(mesh, 0x5c) == 0) {
        glVertexAttribPointer(position, 3, 0x1406, 0, 0, meshPtr(mesh, 0x4));
        glVertexAttribPointer(texcoord, 2, 0x1406, 0, 0, meshPtr(mesh, 0x8));
    } else {
        glBindBuffer(0x8892, meshU32(mesh, 0x60));
        glVertexAttribPointer(position, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, meshU32(mesh, 0x68));
        glVertexAttribPointer(texcoord, 2, 0x1406, 0, 0, 0);
    }
}

void TextureAlphaTestShader::ConnectShaderComponents(int program, int index)
{
    int (*getUniformLocation)(int, const char *) = glGetUniformLocation;
    this->uTextureLoc[index] = getUniformLocation(program, "u_Texture");
    this->aPositionLoc[index] = glGetAttribLocation(program, "a_Position");
    this->aTexCoordLoc[index] = glGetAttribLocation(program, "a_TexCoord");
    this->uMVPMatrixLoc[index] = getUniformLocation(program, "u_MVPMatrix");
    this->uColorLoc[index] = getUniformLocation(program, "u_Color");
    this->uLightPosLoc[index] = getUniformLocation(program, "u_LightPos");
    this->uAmbientLoc[index] = getUniformLocation(program, "u_Ambient");
    this->uDiffuseLoc[index] = getUniformLocation(program, "u_Diffuse");
    this->uSamplerLoc[index] = getUniformLocation(program, "u_Sampler");
    this->uFogColorLoc[index] = getUniformLocation(program, "u_FogColor");

    glUseProgram(program);
    glUniform1i(this->uTextureLoc[index], 0);
}

TextureAlphaTestShader::TextureAlphaTestShader()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    this->field_0x0 = (void *)(g_TextureAlphaTestShader_vtable + 8);
    g_TextureAlphaTestShader_staticDest = g_TextureAlphaTestShader_staticSource;
    this->name = "TextureAlphaTestShader";
}

TextureAlphaTestShader::~TextureAlphaTestShader()
{
    ((ShaderBaseStruct *)this)->~ShaderBaseStruct();
    ::operator delete(this);
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
    glUseProgram(this->field_0x4);
}
