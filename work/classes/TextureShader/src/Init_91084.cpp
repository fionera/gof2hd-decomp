#include "class.h"

namespace AbyssEngine {

void TextureShader::Init(Engine *)
{
    const char *vertexShader = "TextureShader.vsh";
    uint32_t program = ShaderBaseStruct_ES2LoadProgram(this, vertexShader, "TextureShader.fsh");
    field<uint32_t>(this, 0x04) = program;
    ConnectShaderComponents(program, 0);

    program = ShaderBaseStruct_ES2LoadProgram(this, vertexShader, "TextureShaderExt.fsh");
    field<uint32_t>(this, 0x20) = program;
    return ConnectShaderComponents(program, 1);
}

} // namespace AbyssEngine
