#include "class.h"

namespace AbyssEngine {

void NoTexShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("NoTexShader.vsh", "NoTexShader.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");

    field_i32(this, 0x24) = glGetUniformLocation(field_i32(this, 0x04), "u_mvp");
    field_i32(this, 0x28) = glGetUniformLocation(field_i32(this, 0x04), "u_color");

    return glUseProgram(field_i32(this, 0x04));
}

} // namespace AbyssEngine
