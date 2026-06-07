#include "class.h"

namespace AbyssEngine {

void SandboxShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("SandboxShader.vsh", "SandboxShader.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_normal");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a_tangent");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a_binormal");
    field_i32(this, 0x30) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");

    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u_a");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u_b");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u_c");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u_d");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u_e");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u_f");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u_g");

    glUseProgram(field_i32(this, 0x04));
    glUniform1i(field_i32(this, 0x44), 0);
    return glUniform1i(field_i32(this, 0x48), 1);
}

} // namespace AbyssEngine
