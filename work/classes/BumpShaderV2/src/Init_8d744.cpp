#include "class.h"

namespace AbyssEngine {

// AbyssEngine::BumpShaderV2::Init(AbyssEngine::Engine*)
void BumpShaderV2::Init(Engine *engine)
{
    (void)engine;

    int program = ShaderBaseStruct_ES2LoadProgram(this, "BumpShaderV2.vsh", "BumpShaderV2.fsh");
    this->f_4 = program;

    this->f_20 = glGetAttribLocation(program, "a_position");
    this->f_24 = glGetAttribLocation(this->f_4, "a_normal");
    this->f_28 = glGetAttribLocation(this->f_4, "a_texCoord");
    this->f_2c = glGetAttribLocation(this->f_4, "a_tangent");
    this->f_30 = glGetAttribLocation(this->f_4, "a_binormal");

    this->f_38 = glGetUniformLocation(this->f_4, "u_mvpMatrix");
    this->f_3c = glGetUniformLocation(this->f_4, "u_normalMatrix");
    this->f_40 = glGetUniformLocation(this->f_4, "u_lightDir");
    this->f_44 = glGetUniformLocation(this->f_4, "u_eyePos");
    this->f_48 = glGetUniformLocation(this->f_4, "u_texture0");
    this->f_4c = glGetUniformLocation(this->f_4, "u_texture1");
    this->f_50 = glGetUniformLocation(this->f_4, "u_ambient");
    this->f_54 = glGetUniformLocation(this->f_4, "u_diffuse");
    this->f_58 = glGetUniformLocation(this->f_4, "u_specular");
    this->f_5c = glGetUniformLocation(this->f_4, "u_emissive");

    glUseProgram(this->f_4);
    for (int i = 0; i != 2; i++) {
        int loc = i32(this, i * 4 + 0x48);
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

} // namespace AbyssEngine
