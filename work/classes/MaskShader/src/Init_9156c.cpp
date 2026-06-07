#include "class.h"

namespace AbyssEngine {

void MaskShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("MaskShader.vsh", "MaskShader.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a_color");

    field_i32(this, 0x2c) = glGetUniformLocation(field_i32(this, 0x04), "u_mvpMatrix");
    field_i32(this, 0x30) = glGetUniformLocation(field_i32(this, 0x04), "u_texture0");
    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u_texture1");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u_color");

    glUseProgram(field_i32(this, 0x04));
    for (int i = 0; i != 2; i++) {
        int loc = field_i32(this, i * 4 + 0x30);
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

} // namespace AbyssEngine
