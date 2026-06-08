#include "class.h"

namespace AbyssEngine {

// AbyssEngine::BumpShaderV2::Init(AbyssEngine::Engine*)
void BumpShaderV2::Init(Engine *engine)
{
    (void)engine;

    int program = ShaderBaseStruct_ES2LoadProgram(this, "BumpShaderV2.vsh", "BumpShaderV2.fsh");
    i32(this, 0x04) = program;

    i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    i32(this, 0x24) = glGetAttribLocation(i32(this, 0x04), "a_normal");
    i32(this, 0x28) = glGetAttribLocation(i32(this, 0x04), "a_texCoord");
    i32(this, 0x2c) = glGetAttribLocation(i32(this, 0x04), "a_tangent");
    i32(this, 0x30) = glGetAttribLocation(i32(this, 0x04), "a_binormal");

    i32(this, 0x38) = glGetUniformLocation(i32(this, 0x04), "u_mvpMatrix");
    i32(this, 0x3c) = glGetUniformLocation(i32(this, 0x04), "u_normalMatrix");
    i32(this, 0x40) = glGetUniformLocation(i32(this, 0x04), "u_lightDir");
    i32(this, 0x44) = glGetUniformLocation(i32(this, 0x04), "u_eyePos");
    i32(this, 0x48) = glGetUniformLocation(i32(this, 0x04), "u_texture0");
    i32(this, 0x4c) = glGetUniformLocation(i32(this, 0x04), "u_texture1");
    i32(this, 0x50) = glGetUniformLocation(i32(this, 0x04), "u_ambient");
    i32(this, 0x54) = glGetUniformLocation(i32(this, 0x04), "u_diffuse");
    i32(this, 0x58) = glGetUniformLocation(i32(this, 0x04), "u_specular");
    i32(this, 0x5c) = glGetUniformLocation(i32(this, 0x04), "u_emissive");

    glUseProgram(i32(this, 0x04));
    for (int i = 0; i != 2; i++) {
        int loc = i32(this, i * 4 + 0x48);
        if (loc >= 0)
            glUniform1i(loc, i);
    }
}

} // namespace AbyssEngine
