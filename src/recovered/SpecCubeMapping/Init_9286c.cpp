#include "class.h"

namespace AbyssEngine {

void SpecCubeMapping::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->LoadBindShader("SpecCubeMapping.vsh", "SpecCubeMapping.fsh");
    field_i32(this, 0x04) = program;
    if (program == 0) {
        program = ((ShaderBaseStruct *)this)->ES2LoadProgram("SpecCubeMapping.vsh", "SpecCubeMapping.fsh");
        field_i32(this, 0x04) = program;
    }

    field_i32(this, 0x20) = glGetAttribLocation(program, "a0");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a1");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a2");

    field_i32(this, 0x2c) = glGetUniformLocation(field_i32(this, 0x04), "u0");
    field_i32(this, 0x30) = glGetUniformLocation(field_i32(this, 0x04), "u1");
    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u2");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u3");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u5");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u6");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u7");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u8");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u9");
    field_i32(this, 0x54) = glGetUniformLocation(field_i32(this, 0x04), "u10");
    field_i32(this, 0x58) = glGetUniformLocation(field_i32(this, 0x04), "u11");

    glUseProgram(field_i32(this, 0x04));
    glUniform1i(field_i32(this, 0x40), 0);
    return glUniform1i(field_i32(this, 0x3c), 1);
}

} // namespace AbyssEngine
