#include "class.h"

namespace AbyssEngine {

void GenericShader2::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("GenericShader2.vsh", "GenericShader2.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_normal");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a_tangent");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a_binormal");
    field_i32(this, 0x30) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");

    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u_m0");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u_m1");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u_m2");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u_m3");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u_m4");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u_m5");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u_m6");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u_m7");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u_m8");

    glUseProgram(field_i32(this, 0x04));
    return glUniform1i(field_i32(this, 0x44), 0);
}

} // namespace AbyssEngine
