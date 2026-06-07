#include "class.h"

namespace AbyssEngine {

void BumpRimCubeShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->LoadBindShader("BumpRimCubeShader.vsh", "BumpRimCubeShader.fsh");
    field_i32(this, 0x04) = program;
    if (program == 0) {
        program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpRimCubeShader.vsh", "BumpRimCubeShader.fsh");
        field_i32(this, 0x04) = program;
    }

    field_i32(this, 0x20) = glGetAttribLocation(program, "a0");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a1");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a2");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a3");

    field_i32(this, 0x30) = glGetUniformLocation(field_i32(this, 0x04), "u0");
    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u1");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u2");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u3");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u5");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u6");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u7");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u8");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u9");
    field_i32(this, 0x58) = glGetUniformLocation(field_i32(this, 0x04), "u10");
    field_i32(this, 0x5c) = glGetUniformLocation(field_i32(this, 0x04), "u11");
    field_i32(this, 0x64) = glGetUniformLocation(field_i32(this, 0x04), "u12");
    field_i32(this, 0x6c) = glGetUniformLocation(field_i32(this, 0x04), "u13");
    field_i32(this, 0x60) = glGetUniformLocation(field_i32(this, 0x04), "u14");
    field_i32(this, 0x68) = glGetUniformLocation(field_i32(this, 0x04), "u15");
    field_i32(this, 0x70) = glGetUniformLocation(field_i32(this, 0x04), "u16");
    field_i32(this, 0x74) = glGetUniformLocation(field_i32(this, 0x04), "u17");
    field_i32(this, 0x78) = glGetUniformLocation(field_i32(this, 0x04), "u18");
    field_i32(this, 0x7c) = glGetUniformLocation(field_i32(this, 0x04), "u19");
    field_i32(this, 0x80) = glGetUniformLocation(field_i32(this, 0x04), "u20");
    field_i32(this, 0x84) = glGetUniformLocation(field_i32(this, 0x04), "u21");
    field_i32(this, 0x88) = glGetUniformLocation(field_i32(this, 0x04), "u22");
    field_i32(this, 0x8c) = glGetUniformLocation(field_i32(this, 0x04), "u23");
    field_i32(this, 0x90) = glGetUniformLocation(field_i32(this, 0x04), "u24");
    field_i32(this, 0x94) = glGetUniformLocation(field_i32(this, 0x04), "u25");

    glUseProgram(field_i32(this, 0x04));
    for (int i = 0; i != 2; i++) {
        int loc = field_i32(this, i * 4 + 0x4c);
        if (loc >= 0)
            glUniform1i(loc, i);
    }
    glUniform1i(field_i32(this, 0x54), 7);
}

} // namespace AbyssEngine
