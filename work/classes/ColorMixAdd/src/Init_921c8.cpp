#include "class.h"

namespace AbyssEngine {

void ColorMixAdd::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("ColorMixAdd.vsh", "ColorMixAdd.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x30) = glGetUniformLocation(program, "u0");
    field_i32(this, 0x20) = glGetAttribLocation(field_i32(this, 0x04), "a0");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a1");
    field_i32(this, 0x28) = glGetUniformLocation(field_i32(this, 0x04), "u1");
    field_i32(this, 0x2c) = glGetUniformLocation(field_i32(this, 0x04), "u2");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u3");
    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u5");

    glUseProgram(field_i32(this, 0x04));
    glUniform1i(field_i32(this, 0x30), 0);
}

} // namespace AbyssEngine
