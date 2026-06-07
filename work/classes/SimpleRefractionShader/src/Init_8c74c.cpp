#include "class.h"

extern "C" void _ZN11AbyssEngine6Engine17ActivateRefractFBOEv(AbyssEngine::Engine *engine);

namespace AbyssEngine {

void SimpleRefractionShader::Init(Engine *engine)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram(
        "SimpleRefractionShader.vsh", "SimpleRefractionShader.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_normal");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a_tangent");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");

    field_i32(this, 0x30) = glGetUniformLocation(field_i32(this, 0x04), "u_m0");
    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u_m1");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u_m2");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u_tex0");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u_tex1");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u_m3");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u_m4");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u_m5");

    glActiveTexture(0x84c7);
    _ZN11AbyssEngine6Engine17ActivateRefractFBOEv(engine);

    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u_refract");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u_m6");

    glUseProgram(field_i32(this, 0x04));

    for (int i = 0; i != 2; i = i + 1) {
        if (field_i32(this, 0x38 + i * 4) >= 0)
            glUniform1i(field_i32(this, 0x38 + i * 4), i);
    }
    return glUniform1i(field_i32(this, 0x54), 7);
}

} // namespace AbyssEngine
