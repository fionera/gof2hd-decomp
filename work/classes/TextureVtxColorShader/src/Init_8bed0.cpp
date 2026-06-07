#include "class.h"

namespace AbyssEngine {

void TextureVtxColorShader::Init(Engine *)
{
    const char *vertexShader =
        "attribute highp vec4 a_position;\n"
        "attribute mediump vec2 a_texCoord;\n"
        "attribute mediump vec4 a_VertexColor;\n";
    const char *fragmentShader =
        "precision lowp float;\n"
        "varying mediump vec2 v_texCoord;\n"
        "varying mediump vec4 v_VertexColor;\n";

    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram(vertexShader, fragmentShader);
    field_i32(this, 0x04) = program;
    ConnectShaderComponents(program, 0);

    int fogProgram = ((ShaderBaseStruct *)this)->ES2LoadProgram(vertexShader, "\n");
    field_i32(this, 0x20) = fogProgram;
    return ConnectShaderComponents(fogProgram, 1);
}

} // namespace AbyssEngine
