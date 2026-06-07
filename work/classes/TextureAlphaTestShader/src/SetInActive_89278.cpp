#include "class.h"

namespace AbyssEngine {

void TextureAlphaTestShader::SetInActive()
{
    for (int i = 0; i != 2; i++) {
        char *base = (char *)this + i * 4;
        glDisableVertexAttribArray(*(int *)(base + 0x24));
        glDisableVertexAttribArray(*(int *)(base + 0x2c));
    }
}

} // namespace AbyssEngine
