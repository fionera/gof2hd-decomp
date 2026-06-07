#include "class.h"

namespace AbyssEngine {

void TextureAlphaTestShader::ConnectShaderComponents(int program, int index)
{
    int (*getUniformLocation)(int, const char *) = glGetUniformLocation;
    char *base = (char *)this + index * 4;
    *(int *)(base + 0x44) = getUniformLocation(program, "u_Texture");
    *(int *)(base + 0x24) = glGetAttribLocation(program, "a_Position");
    *(int *)(base + 0x2c) = glGetAttribLocation(program, "a_TexCoord");
    *(int *)(base + 0x34) = getUniformLocation(program, "u_MVPMatrix");
    *(int *)(base + 0x3c) = getUniformLocation(program, "u_Color");
    *(int *)(base + 0x4c) = getUniformLocation(program, "u_LightPos");
    *(int *)(base + 0x54) = getUniformLocation(program, "u_Ambient");
    *(int *)(base + 0x5c) = getUniformLocation(program, "u_Diffuse");
    *(int *)(base + 0x64) = getUniformLocation(program, "u_Sampler");
    *(int *)(base + 0x6c) = getUniformLocation(program, "u_FogColor");

    glUseProgram(program);
    glUniform1i(*(int *)(base + 0x44), 0);
}

} // namespace AbyssEngine
