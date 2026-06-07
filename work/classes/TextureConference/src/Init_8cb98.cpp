#include "class.h"

namespace AbyssEngine {

void TextureConference::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram(
        "TextureConference.vsh", "TextureConference.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x34) = glGetUniformLocation(program, "u_texture");
    field_i32(this, 0x20) = glGetAttribLocation(field_i32(this, 0x04), "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");
    field_i32(this, 0x2c) = glGetUniformLocation(field_i32(this, 0x04), "u_mvp");
    field_i32(this, 0x28) = glGetUniformLocation(field_i32(this, 0x04), "u_color");
    field_i32(this, 0x30) = glGetUniformLocation(field_i32(this, 0x04), "u_offset");

    glUseProgram(field_i32(this, 0x04));
    return glUniform1i(field_i32(this, 0x34), 0);
}

} // namespace AbyssEngine
