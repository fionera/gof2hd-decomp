#include "class.h"

namespace AbyssEngine {

void SpecCubeAlphaMapping::Init(Engine *)
{
    int program = ShaderBaseStruct_LoadBindShader(this, "SpecCubeAlphaMapping.vsh", "SpecCubeAlphaMapping.fsh");
    this->f_4 = program;
    if (program == 0) {
        program = ShaderBaseStruct_ES2LoadProgram(this, "SpecCubeAlphaMapping.vsh", "SpecCubeAlphaMapping2.fsh");
        this->f_4 = program;
    }

    this->f_20 = glGetAttribLocation(program, "a0");
    this->f_24 = glGetAttribLocation(this->f_4, "a1");
    this->f_28 = glGetAttribLocation(this->f_4, "a2");

    this->f_2c = glGetUniformLocation(this->f_4, "u0");
    this->f_30 = glGetUniformLocation(this->f_4, "u1");
    this->f_34 = glGetUniformLocation(this->f_4, "u2");
    this->f_38 = glGetUniformLocation(this->f_4, "u3");
    this->f_40 = glGetUniformLocation(this->f_4, "u4");
    this->f_3c = glGetUniformLocation(this->f_4, "u5");
    this->f_44 = glGetUniformLocation(this->f_4, "u6");
    this->f_48 = glGetUniformLocation(this->f_4, "u7");
    this->f_4c = glGetUniformLocation(this->f_4, "u8");
    this->f_50 = glGetUniformLocation(this->f_4, "u9");
    this->f_54 = glGetUniformLocation(this->f_4, "u10");
    this->f_58 = glGetUniformLocation(this->f_4, "u11");

    glUseProgram(this->f_4);
    glUniform1i(this->f_40, 0);
    return glUniform1i(this->f_3c, 1);
}

} // namespace AbyssEngine
