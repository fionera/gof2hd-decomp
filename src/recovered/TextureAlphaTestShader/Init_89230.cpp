#include "class.h"

namespace AbyssEngine {

void TextureAlphaTestShader::Init(Engine *)
{
    int program = ShaderBaseStruct_ES2LoadProgram((ShaderBaseStruct *)this,
                                                  "TextureAlphaTestShader.vsh",
                                                  "TextureAlphaTestShader.fsh");
    i32(this, 0x04) = program;
    ConnectShaderComponents(program, 0);

    program = ShaderBaseStruct_ES2LoadProgram((ShaderBaseStruct *)this,
                                              "TextureAlphaTestShader.vsh",
                                              "TextureAlphaTestShaderAlpha.fsh");
    i32(this, 0x20) = program;
    ConnectShaderComponents(program, 1);
}

} // namespace AbyssEngine
