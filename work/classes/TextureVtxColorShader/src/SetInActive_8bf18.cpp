#include "class.h"

namespace AbyssEngine {

void TextureVtxColorShader::SetInActive()
{
    void (*disableVertexAttribArray)(uint32_t) = glDisableVertexAttribArray;

    for (int i = 0; i != 2; i++) {
        char *entry = (char *)this + i * 4;
        disableVertexAttribArray(*(uint32_t *)(entry + 0x24));
        disableVertexAttribArray(*(uint32_t *)(entry + 0x2c));
        disableVertexAttribArray(*(uint32_t *)(entry + 0x34));
    }
}

} // namespace AbyssEngine
