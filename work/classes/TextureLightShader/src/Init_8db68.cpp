#include "class.h"

namespace AbyssEngine {

void TextureLightShader::Init(Engine *)
{
    int program = ShaderBaseStruct_ES2LoadProgram(this, "TextureLightShader.vsh", "TextureLightShader.fsh");
    i32(this, 0x04) = program;

    int (*uniformFn)(int, const char *) = glGetUniformLocation;
    i32(this, 0x68) = uniformFn(program, "u0");
    i32(this, 0x6c) = uniformFn(i32(this, 0x04), "u1");

    int (*attribFn)(int, const char *) = glGetAttribLocation;
    i32(this, 0x20) = attribFn(i32(this, 0x04), "a0");
    i32(this, 0x24) = attribFn(i32(this, 0x04), "a1");

    i32(this, 0x28) = uniformFn(i32(this, 0x04), "u2");
    i32(this, 0x2c) = uniformFn(i32(this, 0x04), "u3");
    i32(this, 0x30) = uniformFn(i32(this, 0x04), "u4");
    i32(this, 0x34) = uniformFn(i32(this, 0x04), "u5");
    i32(this, 0x38) = uniformFn(i32(this, 0x04), "u6");
    i32(this, 0x3c) = uniformFn(i32(this, 0x04), "u7");
    i32(this, 0x64) = uniformFn(i32(this, 0x04), "u8");
    i32(this, 0x44) = uniformFn(i32(this, 0x04), "u9");
    i32(this, 0x40) = uniformFn(i32(this, 0x04), "u10");
    i32(this, 0x48) = uniformFn(i32(this, 0x04), "u11");
    i32(this, 0x50) = uniformFn(i32(this, 0x04), "u12");
    i32(this, 0x58) = uniformFn(i32(this, 0x04), "u13");
    i32(this, 0x60) = uniformFn(i32(this, 0x04), "u14");
    i32(this, 0x4c) = uniformFn(i32(this, 0x04), "u15");
    i32(this, 0x54) = uniformFn(i32(this, 0x04), "u16");
    i32(this, 0x5c) = uniformFn(i32(this, 0x04), "u17");

    glUseProgram(i32(this, 0x04));
    return glUniform1i(i32(this, 0x44), 0);
}

} // namespace AbyssEngine
