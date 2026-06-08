#include "class.h"

namespace AbyssEngine {

void TextureAlphaTestShader::UseShader(bool)
{
    if (g_TextureAlphaTestShader_useAlphaProgram != 0) {
        int program = this->f_20;
        if (program != 0) {
            glUseProgram(program);
            return;
        }
    }
    glUseProgram(this->f_4);
}

} // namespace AbyssEngine
