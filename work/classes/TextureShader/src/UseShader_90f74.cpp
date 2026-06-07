#include "class.h"

namespace AbyssEngine {

void TextureShader::UseShader(bool)
{
    if (g_TextureShader_hasSecondProgram != 0) {
        uint32_t program = field<uint32_t>(this, 0x20);
        if (program != 0) {
            return glUseProgram(program);
        }
    }
    return glUseProgram(field<uint32_t>(this, 0x04));
}

} // namespace AbyssEngine
