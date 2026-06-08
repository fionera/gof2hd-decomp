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
    this->f_4 = program;

    this->f_20 = glGetAttribLocation(program, "a0");
    this->f_24 = glGetAttribLocation(this->f_4, "a1");
    this->f_28 = glGetAttribLocation(this->f_4, "a2");

    this->f_2c = glGetUniformLocation(this->f_4, "u0");
    this->f_30 = glGetUniformLocation(this->f_4, "u1");
    this->f_34 = glGetUniformLocation(this->f_4, "u2");
    this->f_38 = glGetUniformLocation(this->f_4, "u3");
    this->f_3c = glGetUniformLocation(this->f_4, "u4");
    this->f_40 = glGetUniformLocation(this->f_4, "u5");
    this->f_44 = glGetUniformLocation(this->f_4, "u6");
    this->f_48 = glGetUniformLocation(this->f_4, "u7");
    this->f_4c = glGetUniformLocation(this->f_4, "u8");
    this->f_54 = glGetUniformLocation(this->f_4, "u9");
    this->f_5c = glGetUniformLocation(this->f_4, "u10");
    this->f_50 = glGetUniformLocation(this->f_4, "u11");
    this->f_58 = glGetUniformLocation(this->f_4, "u12");
    this->f_60 = glGetUniformLocation(this->f_4, "u13");
    this->f_64 = glGetUniformLocation(this->f_4, "u14");
    this->f_68 = glGetUniformLocation(this->f_4, "u15");
    this->f_6c = glGetUniformLocation(this->f_4, "u16");
    this->f_70 = glGetUniformLocation(this->f_4, "u17");
    this->f_74 = glGetUniformLocation(this->f_4, "u18");
    this->f_78 = glGetUniformLocation(this->f_4, "u19");
    this->f_7c = glGetUniformLocation(this->f_4, "u20");
    this->f_80 = glGetUniformLocation(this->f_4, "u21");

    glUseProgram(this->f_4);
    glUniform1i(this->f_40, 0);
    BumpShader_setSampler(this->f_44, 7);
}

} // namespace AbyssEngine
