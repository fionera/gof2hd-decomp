#include "class.h"

namespace AbyssEngine {

void PulseShader::Init(Engine *)
{
    int program = ShaderBaseStruct_ES2LoadProgram(this, "PulseShader.vsh", "PulseShader.fsh");
    i32(this, 0x04) = program;

    i32(this, 0x20) = glGetAttribLocation(program, "a0");
    i32(this, 0x24) = glGetAttribLocation(i32(this, 0x04), "a1");
    i32(this, 0x28) = glGetAttribLocation(i32(this, 0x04), "a2");
    i32(this, 0x2c) = glGetAttribLocation(i32(this, 0x04), "a3");
    i32(this, 0x30) = glGetAttribLocation(i32(this, 0x04), "a4");

    i32(this, 0x34) = glGetUniformLocation(i32(this, 0x04), "u0");
    i32(this, 0x38) = glGetUniformLocation(i32(this, 0x04), "u1");
    i32(this, 0x3c) = glGetUniformLocation(i32(this, 0x04), "u2");
    i32(this, 0x40) = glGetUniformLocation(i32(this, 0x04), "u3");
    i32(this, 0x58) = glGetUniformLocation(i32(this, 0x04), "u4");
    i32(this, 0x44) = glGetUniformLocation(i32(this, 0x04), "u5");
    i32(this, 0x48) = glGetUniformLocation(i32(this, 0x04), "u6");
    i32(this, 0x4c) = glGetUniformLocation(i32(this, 0x04), "u7");
    i32(this, 0x50) = glGetUniformLocation(i32(this, 0x04), "u8");
    i32(this, 0x54) = glGetUniformLocation(i32(this, 0x04), "u9");

    glUseProgram(i32(this, 0x04));
    return glUniform1i(i32(this, 0x44), 0);
}

} // namespace AbyssEngine
