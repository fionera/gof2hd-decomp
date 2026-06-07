#include "class.h"

namespace AbyssEngine {

void TexOnlyShader::Init(Engine *)
{
    uint32_t program = ShaderBaseStruct_ES2LoadProgram((ShaderBaseStruct *)this, "TexOnlyShader.vsh",
                                                       "TexOnlyShader.fsh");
    field<uint32_t>(this, 0x4) = program;

    field<int>(this, 0x20) = glGetAttribLocation(program, "a_position");
    field<int>(this, 0x24) = glGetAttribLocation(field<uint32_t>(this, 0x4), "a_texCoord");
    field<int>(this, 0x28) = glGetUniformLocation(field<uint32_t>(this, 0x4), "u_WorldMatrix");
    field<int>(this, 0x2c) = glGetUniformLocation(field<uint32_t>(this, 0x4), "s_texture");

    glUseProgram(field<uint32_t>(this, 0x4));
    return glUniform1i(field<int>(this, 0x2c), 0);
}

} // namespace AbyssEngine
