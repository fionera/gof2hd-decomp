#include "class.h"

namespace AbyssEngine {

void TextureShader::SetInActive()
{
    for (uint32_t i = 0; i != 2; i += 1) {
        char *slot = bytes(this) + i * 4;
        glDisableVertexAttribArray(*(uint32_t *)(slot + 0x24));
        glDisableVertexAttribArray(*(uint32_t *)(slot + 0x2c));
    }
}

} // namespace AbyssEngine
