#include "class.h"

namespace AbyssEngine {

void TextureLightShader::Init(Engine *)
{
    int program = ShaderBaseStruct_ES2LoadProgram(this, "TextureLightShader.vsh", "TextureLightShader.fsh");
    this->f_4 = program;

    int (*uniformFn)(int, const char *) = glGetUniformLocation;
    this->f_68 = uniformFn(program, "u0");
    this->f_6c = uniformFn(this->f_4, "u1");

    int (*attribFn)(int, const char *) = glGetAttribLocation;
    this->f_20 = attribFn(this->f_4, "a0");
    this->f_24 = attribFn(this->f_4, "a1");

    this->f_28 = uniformFn(this->f_4, "u2");
    this->f_2c = uniformFn(this->f_4, "u3");
    this->f_30 = uniformFn(this->f_4, "u4");
    this->f_34 = uniformFn(this->f_4, "u5");
    this->f_38 = uniformFn(this->f_4, "u6");
    this->f_3c = uniformFn(this->f_4, "u7");
    this->f_64 = uniformFn(this->f_4, "u8");
    this->f_44 = uniformFn(this->f_4, "u9");
    this->f_40 = uniformFn(this->f_4, "u10");
    this->f_48 = uniformFn(this->f_4, "u11");
    this->f_50 = uniformFn(this->f_4, "u12");
    this->f_58 = uniformFn(this->f_4, "u13");
    this->f_60 = uniformFn(this->f_4, "u14");
    this->f_4c = uniformFn(this->f_4, "u15");
    this->f_54 = uniformFn(this->f_4, "u16");
    this->f_5c = uniformFn(this->f_4, "u17");

    glUseProgram(this->f_4);
    return glUniform1i(this->f_44, 0);
}

} // namespace AbyssEngine
