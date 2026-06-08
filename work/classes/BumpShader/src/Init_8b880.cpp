#include "class.h"

// Final indirect call in Init: an engine glUniform1i-style helper (DAT_001ab778).
extern "C" void BumpShader_setSampler(int location, int unit);

namespace AbyssEngine {

// AbyssEngine::BumpShader::Init(AbyssEngine::Engine*)
//   Loads the program, then resolves every attribute/uniform location into this+0x20..0x80,
//   activates the program and binds sampler u(0x40)=0, sampler u(0x44)=7.
void BumpShader::Init(Engine *)
{
    int program = ShaderBaseStruct_ES2LoadProgram(this, "BumpShader.vsh", "BumpShader.fsh");
    i32(this, 0x04) = program;

    i32(this, 0x20) = glGetAttribLocation(program, "a0");
    i32(this, 0x24) = glGetAttribLocation(i32(this, 0x04), "a1");
    i32(this, 0x28) = glGetAttribLocation(i32(this, 0x04), "a2");

    i32(this, 0x2c) = glGetUniformLocation(i32(this, 0x04), "u0");
    i32(this, 0x30) = glGetUniformLocation(i32(this, 0x04), "u1");
    i32(this, 0x34) = glGetUniformLocation(i32(this, 0x04), "u2");
    i32(this, 0x38) = glGetUniformLocation(i32(this, 0x04), "u3");
    i32(this, 0x3c) = glGetUniformLocation(i32(this, 0x04), "u4");
    i32(this, 0x40) = glGetUniformLocation(i32(this, 0x04), "u5");
    i32(this, 0x44) = glGetUniformLocation(i32(this, 0x04), "u6");
    i32(this, 0x48) = glGetUniformLocation(i32(this, 0x04), "u7");
    i32(this, 0x4c) = glGetUniformLocation(i32(this, 0x04), "u8");
    i32(this, 0x54) = glGetUniformLocation(i32(this, 0x04), "u9");
    i32(this, 0x5c) = glGetUniformLocation(i32(this, 0x04), "u10");
    i32(this, 0x50) = glGetUniformLocation(i32(this, 0x04), "u11");
    i32(this, 0x58) = glGetUniformLocation(i32(this, 0x04), "u12");
    i32(this, 0x60) = glGetUniformLocation(i32(this, 0x04), "u13");
    i32(this, 0x64) = glGetUniformLocation(i32(this, 0x04), "u14");
    i32(this, 0x68) = glGetUniformLocation(i32(this, 0x04), "u15");
    i32(this, 0x6c) = glGetUniformLocation(i32(this, 0x04), "u16");
    i32(this, 0x70) = glGetUniformLocation(i32(this, 0x04), "u17");
    i32(this, 0x74) = glGetUniformLocation(i32(this, 0x04), "u18");
    i32(this, 0x78) = glGetUniformLocation(i32(this, 0x04), "u19");
    i32(this, 0x7c) = glGetUniformLocation(i32(this, 0x04), "u20");
    i32(this, 0x80) = glGetUniformLocation(i32(this, 0x04), "u21");

    glUseProgram(i32(this, 0x04));
    glUniform1i(i32(this, 0x40), 0);
    BumpShader_setSampler(i32(this, 0x44), 7);
}

} // namespace AbyssEngine
