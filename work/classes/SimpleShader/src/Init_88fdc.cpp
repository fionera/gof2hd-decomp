#include "class.h"

namespace AbyssEngine {

void SimpleShader::Init(Engine *)
{
    uint32_t program = ShaderBaseStruct_ES2LoadProgram((ShaderBaseStruct *)this, "SimpleShader.vsh", "SimpleShader.fsh");
    field_u32(this, 0x4) = program;
    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetUniformLocation(field_u32(this, 0x4), "u_WorldMatrix");
    field_i32(this, 0x28) = glGetUniformLocation(field_u32(this, 0x4), "u_color");
    return glUseProgram(field_u32(this, 0x4));
}

} // namespace AbyssEngine
