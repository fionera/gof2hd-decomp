#include "class.h"

namespace AbyssEngine {

void DrawFBOShader::SetInActive()
{
    char *self = (char *)this;
    glDisableVertexAttribArray(*(uint32_t *)(self + 0x48));
    return glDisableVertexAttribArray(*(uint32_t *)(self + 0x50));
}

} // namespace AbyssEngine
