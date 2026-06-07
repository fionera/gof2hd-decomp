#include "class.h"

namespace AbyssEngine {

void DrawFBOShader::Init(Engine *)
{
    char *self = (char *)this;
    uint32_t program = ES2LoadProgram("DrawFBOShader.vsh", "DrawFBOShader.fsh");
    *(uint32_t *)(self + 0x4) = program;

    *(int32_t *)(self + 0x48) = glGetAttribLocation(program, "position");
    *(int32_t *)(self + 0x50) = glGetAttribLocation(*(uint32_t *)(self + 0x4), "texCoord");
    *(int32_t *)(self + 0x4c) = glGetUniformLocation(*(uint32_t *)(self + 0x4), "worldViewMatrix");
    *(int32_t *)(self + 0x54) = glGetUniformLocation(*(uint32_t *)(self + 0x4), "texture");

    glUseProgram(*(uint32_t *)(self + 0x4));
    return glUniform1i(*(int32_t *)(self + 0x54), 0);
}

} // namespace AbyssEngine
