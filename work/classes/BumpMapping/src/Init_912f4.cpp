#include "class.h"

namespace AbyssEngine {

void BumpMapping::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpMapping.vsh", "BumpMapping.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_normal");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a_tangent");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a_binormal");
    field_i32(this, 0x30) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");

    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u_mvpMatrix");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u_lightDir");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u_texture");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u_normalMap");

    glUseProgram(field_i32(this, 0x04));
    glUniform1i(field_i32(this, 0x3c), 0);
    return glUniform1i(field_i32(this, 0x40), 1);
}

} // namespace AbyssEngine
