#include "class.h"

namespace AbyssEngine {

void TexOnlyShader::SetInActive()
{
    glDisableVertexAttribArray(field<uint32_t>(this, 0x20));
    return glDisableVertexAttribArray(field<uint32_t>(this, 0x24));
}

} // namespace AbyssEngine
