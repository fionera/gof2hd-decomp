#include "class.h"

namespace AbyssEngine {

void TextureVtxColorShader::ConnectShaderComponents(uint32_t program, int index)
{
    int (*getUniformLocation)(uint32_t, const char *) = glGetUniformLocation;
    int (*getAttribLocation)(uint32_t, const char *) = glGetAttribLocation;
    char *entry = (char *)this + index * 4;

    *(int *)(entry + 0x4c) = getUniformLocation(program, "s_texture");
    *(int *)(entry + 0x24) = getAttribLocation(program, "a_position");
    *(int *)(entry + 0x2c) = getAttribLocation(program, "a_texCoord");
    *(int *)(entry + 0x34) = getAttribLocation(program, "a_VertexColor");
    *(int *)(entry + 0x3c) = getUniformLocation(program, "u_WorldMatrix");
    *(int *)(entry + 0x44) = getUniformLocation(program, "glColor");
    *(int *)(entry + 0x8c) = getUniformLocation(program, "u_DarkenValue");
    *(int *)(entry + 0x64) = getUniformLocation(program, "u_fogColor");
    *(int *)(entry + 0x6c) = getUniformLocation(program, "u_fogMaxDist");
    *(int *)(entry + 0x74) = getUniformLocation(program, "u_fogMinDist");
    *(int *)(entry + 0x7c) = getUniformLocation(program, "u_EnableFog");
    *(int *)(entry + 0x84) = getUniformLocation(program, "u_eyeposmodel");
    *(int *)(entry + 0x54) = getUniformLocation(program, "u_UVAnimation");
    *(int *)(entry + 0x5c) = getUniformLocation(program, "u_UvMatrix");

    glUseProgram(program);
    return glUniform1i(*(int *)(entry + 0x4c), 0);
}

} // namespace AbyssEngine
