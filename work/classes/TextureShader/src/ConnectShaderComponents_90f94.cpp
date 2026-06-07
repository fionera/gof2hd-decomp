#include "class.h"

namespace AbyssEngine {

void TextureShader::ConnectShaderComponents(uint32_t program, int slot)
{
    char *shader = bytes(this) + slot * 4;
    int (*uniformLocation)(uint32_t, const char *) = glGetUniformLocation;

    *(int *)(shader + 0x44) = uniformLocation(program, "texture");
    *(int *)(shader + 0x24) = glGetAttribLocation(program, "position");
    *(int *)(shader + 0x2c) = glGetAttribLocation(program, "texcoord");
    *(int *)(shader + 0x34) = uniformLocation(program, "mvp");
    *(int *)(shader + 0x3c) = uniformLocation(program, "color");
    *(int *)(shader + 0x5c) = uniformLocation(program, "light");
    *(int *)(shader + 0x64) = uniformLocation(program, "fogNear");
    *(int *)(shader + 0x6c) = uniformLocation(program, "fogFar");
    *(int *)(shader + 0x74) = uniformLocation(program, "activeTexture");
    *(int *)(shader + 0x7c) = uniformLocation(program, "fogColor");
    *(int *)(shader + 0x84) = uniformLocation(program, "alpha");
    *(int *)(shader + 0x54) = uniformLocation(program, "textureMode");
    *(int *)(shader + 0x4c) = uniformLocation(program, "worldView");

    glUseProgram(program);
    return glUniform1i(*(int *)(shader + 0x44), 0);
}

} // namespace AbyssEngine
