#include "class.h"

namespace AbyssEngine {

void VertexColorAlphaTextureShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("VCATShader.vsh", "VCATShader.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x34) = glGetAttribLocation(program, "a0");
    field_i32(this, 0x20) = glGetAttribLocation(field_i32(this, 0x04), "a1");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a2");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a3");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a4");
    field_i32(this, 0x30) = glGetAttribLocation(field_i32(this, 0x04), "a5");

    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u0");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u1");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u2");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u3");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u5");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u6");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u7");
    field_i32(this, 0x58) = glGetUniformLocation(field_i32(this, 0x04), "u8");

    glUseProgram(field_i32(this, 0x04));
    return glUniform1i(field_i32(this, 0x48), 0);
}

} // namespace AbyssEngine
