#include "class.h"

namespace AbyssEngine {

void TextureAlphaTestShader::UseShader(bool)
{
    if (g_TextureAlphaTestShader_useAlphaProgram != 0) {
        int program = i32(this, 0x20);
        if (program != 0) {
            glUseProgram(program);
            return;
        }
    }
    glUseProgram(i32(this, 0x04));
}

} // namespace AbyssEngine
