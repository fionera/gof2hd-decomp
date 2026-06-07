#include "class.h"

namespace AbyssEngine {

void PostBWShader::Init(Engine *)
{
    uint32_t program = ES2LoadProgram(
        "attribute vec4 a_Position;attribute vec2 a_TexCoord;varying vec2 v_TexCoord;uniform mat4 u_MVPMatrix;void main(){gl_Position=u_MVPMatrix*a_Position;v_TexCoord=a_TexCoord;}",
        "precision mediump float;varying vec2 v_TexCoord;uniform sampler2D s_Texture;void main(){vec4 c=texture2D(s_Texture,v_TexCoord);float g=(c.r+c.g+c.b)/3.0;gl_FragColor=vec4(g,g,g,c.a);}");
    field_u32(this, 0x4) = program;
    field_i32(this, 0x20) = glGetAttribLocation(program, "a_Position");
    field_i32(this, 0x28) = glGetAttribLocation(field_u32(this, 0x4), "a_TexCoord");
    field_i32(this, 0x24) = glGetUniformLocation(field_u32(this, 0x4), "u_MVPMatrix");
    field_i32(this, 0x2c) = glGetUniformLocation(field_u32(this, 0x4), "s_Texture");
    glUseProgram(field_u32(this, 0x4));
    return glUniform1i(field_i32(this, 0x2c), 0);
}

} // namespace AbyssEngine
